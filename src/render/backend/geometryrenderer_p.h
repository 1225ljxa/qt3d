/****************************************************************************
**
** Copyright (C) 2015 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QT3DRENDER_RENDER_GEOMETRYRENDERER_H
#define QT3DRENDER_RENDER_GEOMETRYRENDERER_H

#include <Qt3DCore/qbackendnode.h>
#include <Qt3DRenderer/qgeometryrenderer.h>
#include <Qt3DRenderer/qgeometryfunctor.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

namespace Render {

class GeometryRendererManager;

class Q_AUTOTEST_EXPORT GeometryRenderer : public Qt3D::QBackendNode
{
public:
    GeometryRenderer();
    ~GeometryRenderer();

    void cleanup();
    void setManager(GeometryRendererManager *manager);
    void updateFromPeer(Qt3D::QNode *peer) Q_DECL_OVERRIDE;
    void sceneChangeEvent(const Qt3D::QSceneChangePtr &e) Q_DECL_OVERRIDE;
    void executeFunctor();

    inline Qt3D::QNodeId geometryId() const { return m_geometryId; }
    inline int instanceCount() const { return m_instanceCount; }
    inline int primitiveCount() const { return m_primitiveCount; }
    inline int baseVertex() const { return m_baseVertex; }
    inline int baseInstance() const { return m_baseInstance; }
    inline int restartIndex() const { return m_restartIndex; }
    inline bool primitiveRestart() const { return m_primitiveRestart; }
    inline QGeometryRenderer::PrimitiveType primitiveType() const { return m_primitiveType; }
    inline bool isDirty() const { return m_dirty; }
    inline QGeometryFunctorPtr geometryFunctor() const { return m_functor; }
    void unsetDirty();

private:
    Qt3D::QNodeId m_geometryId;
    int m_instanceCount;
    int m_primitiveCount;
    int m_baseVertex;
    int m_baseInstance;
    int m_restartIndex;
    bool m_primitiveRestart;
    QGeometryRenderer::PrimitiveType m_primitiveType;
    bool m_dirty;
    QGeometryFunctorPtr m_functor;
    GeometryRendererManager *m_manager;
};

class GeometryRendererFunctor : public Qt3D::QBackendNodeFunctor
{
public:
    explicit GeometryRendererFunctor(GeometryRendererManager *manager);
    Qt3D::QBackendNode *create(Qt3D::QNode *frontend, const Qt3D::QBackendNodeFactory *factory) const Q_DECL_OVERRIDE;
    Qt3D::QBackendNode *get(const Qt3D::QNodeId &id) const Q_DECL_OVERRIDE;
    void destroy(const Qt3D::QNodeId &id) const Q_DECL_OVERRIDE;
private:
    GeometryRendererManager *m_manager;
};

} // namespace Render

} // namespace Qt3DRender

QT_END_NAMESPACE

#endif // QT3DRENDER_RENDER_RENDERGEOMETRYRENDERER_H
