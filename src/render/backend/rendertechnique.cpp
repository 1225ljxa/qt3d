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

#include "rendertechnique_p.h"

#include <Qt3DRenderer/qrenderpass.h>
#include <Qt3DRenderer/qtechnique.h>
#include <Qt3DRenderer/qopenglfilter.h>
#include <Qt3DRenderer/rendereraspect.h>
#include <Qt3DRenderer/private/criterionmanager_p.h>
#include <Qt3DRenderer/private/renderer_p.h>
#include <Qt3DRenderer/private/rendercriterion_p.h>
#include <Qt3DRenderer/private/renderpassmanager_p.h>
#include <Qt3DRenderer/private/rendershader_p.h>
#include <Qt3DCore/qaspectmanager.h>
#include <Qt3DCore/qchangearbiter.h>
#include <Qt3DCore/qscenepropertychange.h>

#include <QDebug>

QT_BEGIN_NAMESPACE

namespace Qt3D {
namespace Render {

RenderTechnique::RenderTechnique() :
    m_renderer(Q_NULLPTR),
    m_passCount(0),
    m_openglFilter(new QOpenGLFilter())
{
}

RenderTechnique::~RenderTechnique()
{
    cleanup();
    delete m_openglFilter;
}

void RenderTechnique::cleanup()
{
    if (m_renderer != Q_NULLPTR && !m_techniqueUuid.isNull())
        m_renderer->rendererAspect()->aspectManager()->changeArbiter()->unregisterObserver(this, m_techniqueUuid);
}

void RenderTechnique::setRenderer(Renderer *renderer)
{
    m_renderer = renderer;
    m_parameterPack.setRenderer(m_renderer);
}

void RenderTechnique::setPeer(QTechnique *peer)
{
    QUuid peerUuid;
    if (peer != Q_NULLPTR)
        peerUuid = peer->uuid();
    if (m_techniqueUuid != peerUuid) {
        QChangeArbiter *arbiter = m_renderer->rendererAspect()->aspectManager()->changeArbiter();
        if (!m_techniqueUuid.isNull()) {
            arbiter->unregisterObserver(this, m_techniqueUuid);
            m_parameterPack.clear();
            m_renderPasses.clear();
            m_criteriaList.clear();
       }
        m_techniqueUuid = peerUuid;
        if (!m_techniqueUuid.isNull()) {
            arbiter->registerObserver(this, m_techniqueUuid, NodeAdded|NodeRemoved|NodeUpdated);

            Q_FOREACH (QParameter *p, peer->parameters())
                m_parameterPack.appendParameter(p);
            Q_FOREACH (QRenderPass *rPass, peer->renderPasses())
                appendRenderPass(rPass);
            Q_FOREACH (QCriterion *criterion, peer->criteria())
                appendCriterion(criterion);

            // Copy OpenGLFilter info from frontend OpenGLFilter
            QOpenGLFilter *peerFilter = peer->openGLFilter();
            m_openglFilter->copy(peerFilter);
        }
    }
}

void RenderTechnique::sceneChangeEvent(const QSceneChangePtr &e)
{
    QScenePropertyChangePtr propertyChange = qSharedPointerCast<QScenePropertyChange>(e);
    switch (e->type()) {

    case NodeUpdated: {
        if (propertyChange->propertyName() == QByteArrayLiteral("openGLFilter")) {
            QOpenGLFilter *filter = propertyChange->value().value<QOpenGLFilter *>();
            if (filter != Q_NULLPTR) {
                m_openglFilter->copy(filter);
                delete filter;
            }
        }
        break;
    }

    case NodeAdded: {
        if (propertyChange->propertyName() == QByteArrayLiteral("pass")) {
            appendRenderPass(propertyChange->value().value<QRenderPass *>());
        }
        else if (propertyChange->propertyName() == QByteArrayLiteral("parameter")) {
            m_parameterPack.appendParameter(propertyChange->value().value<QParameter*>());
        }
        else if (propertyChange->propertyName() == QByteArrayLiteral("criterion")) {
            appendCriterion(propertyChange->value().value<QCriterion *>());
        }
        break;
    }

    case NodeRemoved: {
        if (propertyChange->propertyName() == QByteArrayLiteral("pass")) {
            removeRenderPass(propertyChange->value().toUuid());
        }
        else if (propertyChange->propertyName() == QByteArrayLiteral("parameter")) {
            m_parameterPack.removeParameter(propertyChange->value().value<QParameter*>());
        }
        else if (propertyChange->propertyName() == QByteArrayLiteral("criterion")) {
            removeCriterion(propertyChange->value().toUuid());
        }
        break;
    }

    default:
        break;
    }
}

const QHash<QString, QVariant> RenderTechnique::parameters() const
{
    return m_parameterPack.namedValues();
}

void RenderTechnique::appendRenderPass(QRenderPass *renderPass)
{
    if (!renderPass)
        return;
    if (!m_renderPasses.contains(renderPass->uuid()))
        m_renderPasses.append(renderPass->uuid());
}

void RenderTechnique::removeRenderPass(const QUuid &renderPassId)
{
    m_renderPasses.removeOne(renderPassId);
}

QList<QUuid> RenderTechnique::criteria() const
{
    return m_criteriaList;
}

QList<QUuid> RenderTechnique::renderPasses() const
{
    return m_renderPasses;
}

QOpenGLFilter *RenderTechnique::openGLFilter() const
{
    return m_openglFilter;
}

QUuid RenderTechnique::techniqueUuid() const
{
    return m_techniqueUuid;
}

void RenderTechnique::appendCriterion(QCriterion *criterion)
{
    if (!m_criteriaList.contains(criterion->uuid()))
        m_criteriaList.append(criterion->uuid());
}

void RenderTechnique::removeCriterion(const QUuid &criterionId)
{
    m_criteriaList.removeOne(criterionId);
}

} // namespace Render
} // namespace Qt3D

QT_END_NAMESPACE
