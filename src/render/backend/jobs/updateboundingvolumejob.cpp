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

#include "updateboundingvolumejob_p.h"

#include <Qt3DRenderer/private/renderer_p.h>
#include <Qt3DRenderer/private/renderentity_p.h>
#include <Qt3DRenderer/private/renderlogging_p.h>
#include <sphere.h>

#include <QElapsedTimer>
#include <QStack>
#include <QThread>

QT_BEGIN_NAMESPACE

namespace Qt3D {
namespace Render {

namespace {

void expandWorldBoundingVolume(Qt3D::Render::RenderEntity *node)
{
    Qt3D::Render::RenderEntity *currentNode = node;
    QStack<int> childIndexStack;
    forever {

        // Find left most leaf node of currentNode
        while (!currentNode && !currentNode->children().isEmpty()) {
            childIndexStack.push(1);
            currentNode = currentNode->children().first();
        }

        if (!currentNode || !currentNode->parent())
            return;

        // Initialize parent bounding volume to be equal to that of the first child
        Qt3D::Render::RenderEntity *parentNode = currentNode->parent();
        Qt3D::Sphere *parentBoundingVolume = parentNode->worldBoundingVolume();
        *(parentBoundingVolume) = *(currentNode->worldBoundingVolume());

        // Expand the parent bounding volume by each of remaining the siblings
        QVector<RenderEntity *> siblings = parentNode->children();
        const int siblingCount = siblings.count();
        for (int i = 1; i < siblingCount; ++i) {
            Qt3D::Sphere *siblingBoundingVolume = siblings.at(i)->worldBoundingVolume();
            parentBoundingVolume->expandToContain(*siblingBoundingVolume);
        }

        // Move to parent's next sibling
        childIndexStack.pop();
        currentNode = Q_NULLPTR;
        if (!childIndexStack.empty() && parentNode->parent()) {
            const int nextSiblingIndex = childIndexStack.top()++;
            QVector<RenderEntity *> parentSiblings = parentNode->parent()->children();
            if (nextSiblingIndex < parentSiblings.size())
                currentNode = parentSiblings.at(nextSiblingIndex);
        }
    }
}

}

UpdateBoundingVolumeJob::UpdateBoundingVolumeJob(RenderEntity *node)
    : m_node(node)
{
}

void UpdateBoundingVolumeJob::run()
{
    // Expand the bounding volumes of each node that has children by the
    // bounding volumes of the children

    // TODO: Implement this using a parallel_for
    qCDebug(Jobs) << "Entering" << Q_FUNC_INFO << QThread::currentThread();
    expandWorldBoundingVolume(m_node);
    qCDebug(Jobs) << "Exiting" << Q_FUNC_INFO << QThread::currentThread();
}

} // namespace Render
} // namespace Qt3D

QT_END_NAMESPACE
