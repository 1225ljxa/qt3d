/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
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

#include "entity_p.h"
#include <Qt3DRenderer/private/managers_p.h>
#include <Qt3DRenderer/private/renderer_p.h>
#include <Qt3DRenderer/qabstractlight.h>
#include <Qt3DRenderer/qlayer.h>
#include <Qt3DRenderer/qmaterial.h>
#include <Qt3DRenderer/qmesh.h>
#include <Qt3DRenderer/private/renderlogging_p.h>
#include <Qt3DRenderer/sphere.h>
#include <Qt3DRenderer/qshaderdata.h>
#include <Qt3DRenderer/qgeometryrenderer.h>
#include <Qt3DRenderer/private/geometryrenderermanager_p.h>

#include <Qt3DCore/qcameralens.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qscenepropertychange.h>
#include <Qt3DCore/qtransform.h>

#include <QMatrix4x4>
#include <QString>

QT_BEGIN_NAMESPACE

using namespace Qt3D;

namespace Qt3DRender {
namespace Render {

Entity::Entity()
    : QBackendNode()
    , m_renderer(Q_NULLPTR)
    , m_localBoundingVolume(new Sphere)
    , m_worldBoundingVolume(new Sphere)
{
}

Entity::~Entity()
{
    cleanup();
}

void Entity::cleanup()
{
    if (m_renderer != Q_NULLPTR) {
        Entity *parentEntity = parent();
        if (parentEntity != Q_NULLPTR)
            parentEntity->removeChildHandle(m_handle);
        for (int i = 0; i < m_childrenHandles.size(); i++)
            m_renderer->renderNodesManager()->release(m_childrenHandles[i]);
        // We need to release using peerUuid otherwise the handle will be cleared
        // but would still remain in the Id to Handle table
        m_renderer->worldMatrixManager()->releaseResource(peerUuid());
        m_worldTransform = HMatrix();

        // Release all component will have to perform their own release when they receive the
        // NodeDeleted/NodeAboutToBeDeleted notification
        qCDebug(Render::RenderNodes) << Q_FUNC_INFO;

        // Clear components
        m_transformComponent = Qt3D::QNodeId();
        m_cameraComponent = Qt3D::QNodeId();
        m_materialComponent = Qt3D::QNodeId();
        m_geometryRendererComponent = Qt3D::QNodeId();
        m_layerComponents.clear();
        m_shaderDataComponents.clear();
    }
    delete m_localBoundingVolume;
    delete m_worldBoundingVolume;
    m_localBoundingVolume = Q_NULLPTR;
    m_worldBoundingVolume = Q_NULLPTR;


}

void Entity::setParentHandle(HEntity parentHandle)
{
    Q_ASSERT(m_renderer);
    // Remove ourselves from previous parent children list
    Entity *parent = m_renderer->renderNodesManager()->data(parentHandle);
    if (parent != Q_NULLPTR && parent->m_childrenHandles.contains(m_handle))
        parent->m_childrenHandles.remove(m_handle);
    m_parentHandle = parentHandle;
    parent = m_renderer->renderNodesManager()->data(parentHandle);
    if (parent != Q_NULLPTR && !parent->m_childrenHandles.contains(m_handle))
        parent->m_childrenHandles.append(m_handle);
}

void Entity::setRenderer(Renderer *renderer)
{
    m_renderer = renderer;
}

void Entity::setHandle(HEntity handle)
{
    m_handle = handle;
}

void Entity::updateFromPeer(Qt3D::QNode *peer)
{
    QEntity *entity = static_cast<QEntity *>(peer);
    const QNodeId parentEntityId = entity->parentEntityId();

    m_objectName = peer->objectName();
    m_worldTransform = m_renderer->worldMatrixManager()->getOrAcquireHandle(peerUuid());

    // TO DO: Suboptimal -> Maybe have a Hash<QComponent, QEntityList> instead
    m_transformComponent = QNodeId();
    m_materialComponent = QNodeId();
    m_cameraComponent = QNodeId();
    m_geometryRendererComponent = QNodeId();
    m_layerComponents.clear();
    m_shaderDataComponents.clear();

    Q_FOREACH (QComponent *comp, entity->components())
        addComponent(comp);

    if (!parentEntityId.isNull()) {
        setParentHandle(m_renderer->renderNodesManager()->lookupHandle(parentEntityId));
    } else {
        qCDebug(Render::RenderNodes) << Q_FUNC_INFO << "No parent entity found for Entity" << peerUuid();
    }
}

void Entity::sceneChangeEvent(const Qt3D::QSceneChangePtr &e)
{
    QScenePropertyChangePtr propertyChange = qSharedPointerCast<QScenePropertyChange>(e);
    switch (e->type()) {

    case ComponentAdded: {
        QNodePtr nodePtr = propertyChange->value().value<QNodePtr>();
        QComponent *component = qobject_cast<QComponent *>(nodePtr.data());
        qCDebug(Render::RenderNodes) << Q_FUNC_INFO << "Component Added" << m_objectName << component->objectName();
        addComponent(component);
        break;
    }

    case ComponentRemoved: {
        QNodeId nodeId = propertyChange->value().value<QNodeId>();
        qCDebug(Render::RenderNodes) << Q_FUNC_INFO << "Component Removed";
        removeComponent(nodeId);
        break;
    }

    default:
        break;
    }
}

void Entity::dump() const
{
    static int depth = 0;
    QString indent(2 * depth++, QChar::fromLatin1(' '));
    qCDebug(Backend) << indent + m_objectName;
    foreach (const Entity *child, children())
        child->dump();
    --depth;
}

Entity *Entity::parent() const
{
    return m_renderer->renderNodesManager()->data(m_parentHandle);
}

void Entity::appendChildHandle(HEntity childHandle)
{
    if (!m_childrenHandles.contains(childHandle)) {
        m_childrenHandles.append(childHandle);
        Entity *child = m_renderer->renderNodesManager()->data(childHandle);
        if (child != Q_NULLPTR)
            child->m_parentHandle = m_handle;
    }
}

void Entity::removeChildHandle(HEntity childHandle)
{
    // TO DO : Check if a QList here wouldn't be more performant
    if (m_childrenHandles.contains(childHandle)) {
        m_childrenHandles.removeAt(m_childrenHandles.indexOf(childHandle));
    }
}

QVector<Entity *> Entity::children() const
{
    QVector<Entity *> childrenVector;
    childrenVector.reserve(m_childrenHandles.size());
    foreach (HEntity handle, m_childrenHandles) {
        Entity *child = m_renderer->renderNodesManager()->data(handle);
        if (child != Q_NULLPTR)
            childrenVector.append(child);
    }
    return childrenVector;
}

QMatrix4x4 *Entity::worldTransform()
{
    return m_renderer->worldMatrixManager()->data(m_worldTransform);
}

const QMatrix4x4 *Entity::worldTransform() const
{
    return m_renderer->worldMatrixManager()->data(m_worldTransform);
}

void Entity::addComponent(Qt3D::QComponent *component)
{
    // The backend element is always created when this method is called
    // If that's not the case something has gone wrong

    if (qobject_cast<Qt3D::QTransform*>(component) != Q_NULLPTR)
        m_transformComponent = component->id();
    else if (qobject_cast<QCameraLens *>(component) != Q_NULLPTR)
        m_cameraComponent = component->id();
    else if (qobject_cast<QLayer *>(component) != Q_NULLPTR)
        m_layerComponents.append(component->id());
    else if (qobject_cast<QMaterial *>(component) != Q_NULLPTR)
        m_materialComponent = component->id();
    else if (qobject_cast<QShaderData *>(component) != Q_NULLPTR)
        m_shaderDataComponents.append(component->id());
    else if (qobject_cast<QGeometryRenderer *>(component) != Q_NULLPTR)
        m_geometryRendererComponent = component->id();
}

void Entity::removeComponent(const Qt3D::QNodeId &nodeId)
{
    if (m_transformComponent == nodeId)
        m_transformComponent = QNodeId();
    else if (m_cameraComponent == nodeId)
        m_cameraComponent = QNodeId();
    else if (m_layerComponents.contains(nodeId))
        m_layerComponents.removeAll(nodeId);
    else if (m_materialComponent == nodeId)
        m_materialComponent = QNodeId();
    else if (m_shaderDataComponents.contains(nodeId))
        m_shaderDataComponents.removeAll(nodeId);
    else if (m_geometryRendererComponent == nodeId)
        m_geometryRendererComponent = QNodeId();
}

template<>
HMaterial Entity::componentHandle<RenderMaterial>() const
{
    return m_renderer->materialManager()->lookupHandle(m_materialComponent);
}

template<>
RenderMaterial *Entity::renderComponent<RenderMaterial>() const
{
    return m_renderer->materialManager()->lookupResource(m_materialComponent);
}

template<>
HCamera Entity::componentHandle<CameraLens>() const
{
    return m_renderer->cameraManager()->lookupHandle(m_cameraComponent);
}

template<>
CameraLens *Entity::renderComponent<CameraLens>() const
{
    return m_renderer->cameraManager()->lookupResource(m_cameraComponent);
}

template<>
HTransform Entity::componentHandle<RenderTransform>() const
{
    return m_renderer->transformManager()->lookupHandle(m_transformComponent);
}

template<>
RenderTransform *Entity::renderComponent<RenderTransform>() const
{
    return m_renderer->transformManager()->lookupResource(m_transformComponent);
}

template<>
HGeometryRenderer Entity::componentHandle<GeometryRenderer>() const
{
    return m_renderer->geometryRendererManager()->lookupHandle(m_geometryRendererComponent);
}

template<>
GeometryRenderer *Entity::renderComponent<GeometryRenderer>() const
{
    return m_renderer->geometryRendererManager()->lookupResource(m_geometryRendererComponent);
}

template<>
Qt3D::QNodeId Entity::componentUuid<RenderTransform>() const { return m_transformComponent; }

template<>
Qt3D::QNodeId Entity::componentUuid<CameraLens>() const { return m_cameraComponent; }

template<>
Qt3D::QNodeId Entity::componentUuid<RenderMaterial>() const { return m_materialComponent; }

template<>
QList<HLayer> Entity::componentsHandle<RenderLayer>() const
{
    QList<HLayer> layerHandles;
    Q_FOREACH (const QNodeId &id, m_layerComponents)
        layerHandles.append(m_renderer->layerManager()->lookupHandle(id));
    return layerHandles;
}

template<>
QList<RenderLayer *> Entity::renderComponents<RenderLayer>() const
{
    QList<RenderLayer *> layers;
    Q_FOREACH (const QNodeId &id, m_layerComponents)
        layers.append(m_renderer->layerManager()->lookupResource(id));
    return layers;
}

template<>
QList<Qt3D::QNodeId> Entity::componentsUuid<RenderLayer>() const { return m_layerComponents; }

template<>
QList<HShaderData> Entity::componentsHandle<RenderShaderData>() const
{
    QList<HShaderData> shaderDataHandles;
    Q_FOREACH (const QNodeId &id, m_shaderDataComponents)
        shaderDataHandles.append(m_renderer->shaderDataManager()->lookupHandle(id));
    return shaderDataHandles;
}

template<>
QList<RenderShaderData *> Entity::renderComponents<RenderShaderData>() const
{
    QList<RenderShaderData *> shaderDatas;
    Q_FOREACH (const QNodeId &id, m_shaderDataComponents)
        shaderDatas.append(m_renderer->shaderDataManager()->lookupResource(id));
    return shaderDatas;
}

template<>
QList<Qt3D::QNodeId> Entity::componentsUuid<RenderShaderData>() const { return m_shaderDataComponents; }

template<>
Qt3D::QNodeId Entity::componentUuid<GeometryRenderer>() const { return m_geometryRendererComponent; }


RenderEntityFunctor::RenderEntityFunctor(Renderer *renderer)
    : m_renderer(renderer)
{
}

Qt3D::QBackendNode *RenderEntityFunctor::create(Qt3D::QNode *frontend, const Qt3D::QBackendNodeFactory *factory) const
{
    HEntity renderNodeHandle = m_renderer->renderNodesManager()->getOrAcquireHandle(frontend->id());
    Entity *entity = m_renderer->renderNodesManager()->data(renderNodeHandle);
    entity->setFactory(factory);
    entity->setRenderer(m_renderer);
    entity->setHandle(renderNodeHandle);
    entity->setPeer(frontend);
    return entity;
}

Qt3D::QBackendNode *RenderEntityFunctor::get(const Qt3D::QNodeId &id) const
{
    return m_renderer->renderNodesManager()->lookupResource(id);
}

void RenderEntityFunctor::destroy(const Qt3D::QNodeId &id) const
{
    m_renderer->renderNodesManager()->releaseResource(id);
}

} // namespace Render
} // namespace Qt3DRender

QT_END_NAMESPACE
