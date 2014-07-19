/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "rendereraspect.h"

#include "rendermesh.h"
#include "meshmanager.h"
#include "meshdatamanager.h"
#include "renderer.h"
#include "renderthread.h"

#include <loadmeshdatajob.h>
#include <updateworldtransformjob.h>
#include <updateboundingvolumejob.h>

#include <Qt3DCore/qnode.h>
#include <Qt3DCore/qaspectmanager.h>

#include <QDebug>
#include <QThread>
#include <QWindow>

QT_BEGIN_NAMESPACE

namespace Qt3D {

RendererAspect::RendererAspect(QObject *parent)
    : QAbstractAspect(QAbstractAspect::AspectRenderer, parent)
    , m_renderThread(new RenderThread)
{
    qRegisterMetaType<QSurface*>("QSurface*");
    qRegisterMetaType< QSharedPointer<QObject> >("QObjectPtr");
    // TO DO : Check whether it would make sense to have the Renderer create a private thread
    // To perform its rendering and remove this one.

    // Won't return until RenderThread and Renderer have been created
    // The Renderer is set to wait the surface with a wait condition
    // Threads modifying the Renderer should be synchronized using the Renderer's mutex
    m_renderThread->waitForStart();
}

QVector<QJobPtr> RendererAspect::jobsToExecute()
{
    // Create jobs that will get exectued by the threadpool
    QVector<QJobPtr> jobs;

    // Create jobs to load in any meshes that are pending
    if (m_renderThread->renderer() != Q_NULLPTR) {
        QHash<QUuid, QAbstractMeshFunctorPtr> meshSources = m_renderThread->renderer()->meshDataManager()->meshesPending();
        QVector<QJobPtr> meshesJobs;
        Q_FOREACH (const QUuid &meshId, meshSources.keys()) {
            Render::LoadMeshDataJobPtr loadMeshJob(new Render::LoadMeshDataJob(meshSources[meshId], meshId));
            loadMeshJob->setRenderer(m_renderThread->renderer());
            meshesJobs.append(loadMeshJob);
        }

        // Create jobs to update transforms and bounding volumes
        Render::UpdateWorldTransformJobPtr worldTransformJob(new Render::UpdateWorldTransformJob(m_renderThread->renderer()->renderSceneRoot()));
        Render::UpdateBoundingVolumeJobPtr boundingVolumeJob(new Render::UpdateBoundingVolumeJob(m_renderThread->renderer()->renderSceneRoot()));

        Q_FOREACH (QJobPtr meshJob, meshesJobs) {
            worldTransformJob->addDependency(meshJob);
            jobs.append(meshJob);
        }
        //    // We can only update bounding volumes once all world transforms are known
        boundingVolumeJob->addDependency(worldTransformJob);

        //    // Add all jobs to queue
        jobs.append(worldTransformJob);
        jobs.append(boundingVolumeJob);

        // Traverse the current framegraph and create jobs to populate
        // RenderBins with RenderCommands
        QVector<QJobPtr> renderBinJobs = m_renderThread->renderer()->createRenderBinJobs();
        // TODO: Add wrapper around ThreadWeaver::Collection
        for (int i = 0; i < renderBinJobs.size(); ++i) {
            QJobPtr renderBinJob = renderBinJobs.at(i);
            renderBinJob->addDependency(boundingVolumeJob);
            jobs.append(renderBinJob);
        }
    }
    return jobs;
}

void RendererAspect::registerAspectHelper(QEntity *rootObject)
{
    Render::Renderer *renderer = m_renderThread->renderer();
    // setSceneGraphRoot is synchronized using the Renderer's mutex
    renderer->setSceneGraphRoot(rootObject);
}

void RendererAspect::unregisterAspectHelper(QEntity *rootObject)
{
    Q_UNUSED(rootObject);
}

void RendererAspect::onInitialize()
{
    Render::Renderer *renderer = m_renderThread->renderer();
    renderer->setRendererAspect(this);
    // setSurface is synchronized using the Renderer's mutex
    renderer->setSurface(aspectManager()->window());
}

void RendererAspect::onCleanup()
{
    //Render::Renderer *renderer = m_renderThread->renderer();
    //QMetaObject::invokeMethod(renderer, "cleanup");
}

} // Qt3D

QT_END_NAMESPACE
