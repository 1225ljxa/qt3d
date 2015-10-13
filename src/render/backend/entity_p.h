/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Copyright (C) 2015 The Qt Company Ltd and/or its subsidiary(-ies).
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

#ifndef QT3DRENDER_RENDER_ENTITY_H
#define QT3DRENDER_RENDER_ENTITY_H

#include <Qt3DRenderer/private/renderer_p.h>
#include <Qt3DRenderer/private/handle_types_p.h>
#include <Qt3DCore/qbackendnode.h>
#include <Qt3DCore/qnodeid.h>
#include <Qt3DCore/qhandle.h>
#include <QVector>

QT_BEGIN_NAMESPACE

class QMatrix4x4;

namespace Qt3D {
class QNode;
class QEntity;
class QComponent;
}

namespace Qt3DRender {

class QRenderAspect;
class Sphere;

namespace Render {

class Renderer;

class Q_AUTOTEST_EXPORT Entity : public Qt3D::QBackendNode
{
public:
    Entity();
    ~Entity();
    void cleanup();

    void setParentHandle(HEntity parentHandle);
    void setRenderer(Renderer *renderer);
    void sceneChangeEvent(const Qt3D::QSceneChangePtr &e) Q_DECL_OVERRIDE;
    void updateFromPeer(Qt3D::QNode *peer) Q_DECL_OVERRIDE;

    void dump() const;

    void  setHandle(HEntity handle);
    HEntity handle() const { return m_handle; }
    Entity *parent() const;
    HEntity parentHandle() const { return m_parentHandle; }

    void appendChildHandle(HEntity childHandle);
    void removeChildHandle(HEntity childHandle);
    QVector<HEntity> childrenHandles() const { return m_childrenHandles; }
    QVector<Entity *> children() const;

    QMatrix4x4 *worldTransform();
    const QMatrix4x4 *worldTransform() const;
    Sphere *localBoundingVolume() { return m_localBoundingVolume; }
    Sphere *worldBoundingVolume() { return m_worldBoundingVolume; }

    void addComponent(Qt3D::QComponent *component);
    void removeComponent(const Qt3D::QNodeId &nodeId);

    template<class Backend, uint INDEXBITS>
    Qt3D::QHandle<Backend, INDEXBITS> componentHandle() const
    {
        return Qt3D::QHandle<Backend, INDEXBITS>();
    }

    template<class Backend, uint INDEXBITS>
    QList<Qt3D::QHandle<Backend, INDEXBITS> > componentsHandle() const
    {
        return QList<Qt3D::QHandle<Backend, INDEXBITS> >();
    }

    template<class Backend>
    Backend *renderComponent() const
    {
        return Q_NULLPTR;
    }

    template<class Backend>
    QList<Backend *> renderComponents() const
    {
        return QList<Backend *>();
    }

    template<class Backend>
    Qt3D::QNodeId componentUuid() const
    {
        return Qt3D::QNodeId();
    }

    template<class Backend>
    QList<Qt3D::QNodeId> componentsUuid() const
    {
        return QList<Qt3D::QNodeId>();
    }

private:

    template<class Frontend, class Backend, class Manager>
    void createRenderComponentHelper(Frontend *frontend, Manager *manager)
    {
        // We index using the Frontend id
        if (!manager->contains(frontend->id())) {
            Backend *backend = manager->getOrCreateResource(frontend->id());
            backend->setRenderer(m_renderer);
            backend->setPeer(frontend);
        }
    }

    Renderer *m_renderer;
    HEntity m_handle;
    HEntity m_parentHandle;
    QVector<HEntity > m_childrenHandles;

    HMatrix m_worldTransform;
    Sphere *m_localBoundingVolume;
    Sphere *m_worldBoundingVolume;

    // Handles to Components
    Qt3D::QNodeId m_transformComponent;
    Qt3D::QNodeId m_materialComponent;
    Qt3D::QNodeId m_cameraComponent;
    QList<Qt3D::QNodeId> m_layerComponents;
    QList<Qt3D::QNodeId> m_shaderDataComponents;
    Qt3D::QNodeId m_geometryRendererComponent;

    QString m_objectName;
};

template<>
HMaterial Entity::componentHandle<Material>() const;

template<>
Material *Entity::renderComponent<Material>() const;

template<>
HCamera Entity::componentHandle<CameraLens>() const;

template<>
CameraLens *Entity::renderComponent<CameraLens>() const;

template<>
HTransform Entity::componentHandle<RenderTransform>() const;

template<>
RenderTransform *Entity::renderComponent<RenderTransform>() const;

template<>
Q_AUTOTEST_EXPORT HGeometryRenderer Entity::componentHandle<GeometryRenderer>() const;

template<>
Q_AUTOTEST_EXPORT GeometryRenderer *Entity::renderComponent<GeometryRenderer>() const;

template<>
Q_AUTOTEST_EXPORT Qt3D::QNodeId Entity::componentUuid<RenderTransform>() const;

template<>
Q_AUTOTEST_EXPORT Qt3D::QNodeId Entity::componentUuid<CameraLens>() const;

template<>
Q_AUTOTEST_EXPORT Qt3D::QNodeId Entity::componentUuid<Material>() const;

template<>
QList<HLayer> Entity::componentsHandle<Layer>() const;

template<>
QList<Layer *> Entity::renderComponents<Layer>() const;

template<>
Q_AUTOTEST_EXPORT QList<Qt3D::QNodeId> Entity::componentsUuid<Layer>() const;

template<>
QList<HShaderData> Entity::componentsHandle<ShaderData>() const;

template<>
QList<ShaderData *> Entity::renderComponents<ShaderData>() const;

template<>
Q_AUTOTEST_EXPORT QList<Qt3D::QNodeId> Entity::componentsUuid<ShaderData>() const;

template<>
Q_AUTOTEST_EXPORT Qt3D::QNodeId Entity::componentUuid<GeometryRenderer>() const;

class RenderEntityFunctor : public Qt3D::QBackendNodeFunctor
{
public:
    explicit RenderEntityFunctor(Renderer *renderer);
    Qt3D::QBackendNode *create(Qt3D::QNode *frontend, const Qt3D::QBackendNodeFactory *factory) const Q_DECL_OVERRIDE;
    Qt3D::QBackendNode *get(const Qt3D::QNodeId &id) const Q_DECL_OVERRIDE;
    void destroy(const Qt3D::QNodeId &id) const Q_DECL_OVERRIDE;

private:
    Renderer *m_renderer;
};

} // namespace Render
} // namespace Qt3DRender

QT_END_NAMESPACE

#endif // QT3DRENDER_RENDER_ENTITY_H
