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

#include "framegraphvisitor_p.h"

#include "framegraphnode_p.h"
#include <Qt3DRenderer/private/renderer_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {
namespace Render {

FrameGraphVisitor::FrameGraphVisitor()
    : m_renderer(Q_NULLPTR)
    , m_jobs(Q_NULLPTR)
    , m_renderviewIndex(0)

{
}

void FrameGraphVisitor::traverse(FrameGraphNode *root,
                                 Renderer *renderer,
                                 QVector<QJobPtr> *jobs)
{
    m_renderer = renderer;
    m_jobs = jobs;
    m_renderviewIndex = 0;

    Q_ASSERT(m_renderer);
    Q_ASSERT(m_jobs);
    Q_ASSERT_X(root, Q_FUNC_INFO, "The FrameGraphRoot is null");

    // Kick off the traversal
    Render::FrameGraphNode *node = root;
    if (node == Q_NULLPTR)
        qCritical() << Q_FUNC_INFO << "FrameGraph is null";
    visit(node);
}

void FrameGraphVisitor::visit(Render::FrameGraphNode *node)
{
    // Recurse to children (if we have any), otherwise if this is a leaf node,
    // initiate a rendering from the current camera
    Q_FOREACH (Render::FrameGraphNode *n, node->children())
        visit(n);
    // Leaf node - create a RenderView ready to be populated
    // TODO: Pass in only framegraph config that has changed from previous
    // index RenderViewJob.
    if (node->childrenHandles().empty()) {
        QJobPtr job = m_renderer->createRenderViewJob(node, m_renderviewIndex++);
        m_jobs->append(job);
    }
}

} // namespace Render
} // namespace Qt3D

QT_END_NAMESPACE
