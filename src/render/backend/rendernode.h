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

#ifndef QT3D_RENDER_RENDERNODE_H
#define QT3D_RENDER_RENDERNODE_H

#include <Qt3DCore/qobserverinterface.h>
#include <Qt3DCore/qhandle.h>
#include <QVector>

QT_BEGIN_NAMESPACE

class QMatrix4x4;

namespace Qt3D {

class RendererAspect;
class Sphere;
class Node;
class Transform;

typedef QHandle<QMatrix4x4, 16> HMatrix;

namespace Render {
class RenderNode;
}
typedef QHandle<Render::RenderNode, 16> HRenderNode;

namespace Render {

class Renderer;
class RenderNodesManager;

class RenderNode : public QObserverInterface
{
public:
    RenderNode();
    ~RenderNode();

    void setTransform(Transform *transform);
    void setParentHandle(HRenderNode parentHandle);
    void setRenderer(Renderer *renderer);
    void sceneChangeEvent(const QSceneChangePtr &e) Q_DECL_OVERRIDE;
    void setFrontEndPeer(Node *peer);

    void dump() const;

    void  setHandle(HRenderNode handle);
    HRenderNode handle() const { return m_handle; }
    RenderNode *parent() const;
    HRenderNode parentHandle() const { return m_parentHandle; }

    void appendChildHandle(HRenderNode childHandle);
    QVector<HRenderNode> childrenHandles() const { return m_childrenHandles; }
    QVector<RenderNode *> children() const;

    QMatrix4x4 *localTransform();
    QMatrix4x4 *worldTransform();
    Sphere *localBoundingVolume() { return m_localBoundingVolume; }
    Sphere *worldBoundingVolume() { return m_worldBoundingVolume; }
    Node *frontEndPeer() const { return m_frontEndPeer; }

private:

    Renderer *m_renderer;
    Transform *m_transform;
    HRenderNode m_handle;
    HRenderNode m_parentHandle;
    QVector<HRenderNode > m_childrenHandles;

    HMatrix m_localTransform;
    HMatrix m_worldTransform;
    Sphere *m_localBoundingVolume;
    Sphere *m_worldBoundingVolume;

    // TODO: Do we want to force this to be an Entity?
    // That would mean forcing an Entity for the root on the main thread's scene
    Node *m_frontEndPeer;

    // TODO: Add pointer to Drawable or references to VBO's and other info needed to draw
};


} // namespace Render
} // namespace Qt3D

QT_END_NAMESPACE

#endif // QT3D_RENDER_RENDERNODE_H
