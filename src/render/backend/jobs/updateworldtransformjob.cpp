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

#include "updateworldtransformjob.h"

#include <Qt3DRenderer/private/renderer_p.h>
#include <Qt3DRenderer/private/renderentity_p.h>
#include <sphere.h>
#include <Qt3DRenderer/private/rendertransform_p.h>
#include "renderlogging.h"
#include <QThread>

QT_BEGIN_NAMESPACE

namespace Qt3D {
namespace Render {

namespace {

void updateWorldTransformAndBounds(Qt3D::Render::RenderEntity *node)
{
    QMatrix4x4 localTransform;
    QMatrix4x4 parentTransform;
    if (node->parent())
        parentTransform = *(node->parent()->worldTransform());

    RenderTransform *nodeTransform = node->renderComponent<RenderTransform>();
    if (nodeTransform != Q_NULLPTR)
        localTransform = nodeTransform->transformMatrix();

    *(node->worldTransform()) = parentTransform * localTransform;
    *(node->worldBoundingVolume()) = node->localBoundingVolume()->transformed(*(node->worldTransform()));

    Q_FOREACH (Qt3D::Render::RenderEntity *child, node->children())
        updateWorldTransformAndBounds(child);
}

}

UpdateWorldTransformJob::UpdateWorldTransformJob(RenderEntity *node)
    : Qt3D::QJob()
    , m_node(node)
{
}

void UpdateWorldTransformJob::run()
{
    // Iterate over each level of hierarchy in our scene
    // and update each node's world transform from its
    // local transform and its parent's world transform

    // TODO: Parallelise this on each level using a parallel_for
    // implementation.

    qCDebug(Jobs) << "Entering" << Q_FUNC_INFO << QThread::currentThread();
    updateWorldTransformAndBounds(m_node);
    qCDebug(Jobs) << "Exiting" << Q_FUNC_INFO << QThread::currentThread();
}

} // namespace Render
} // namespace Qt3D

QT_END_NAMESPACE
