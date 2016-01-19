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

#include "scene3ditem_p.h"
#include "scene3dcleaner_p.h"
#include "scene3dlogging_p.h"
#include "scene3drenderer_p.h"
#include "scene3dsgnode_p.h"

#include <Qt3DCore/QAspectEngine>
#include <Qt3DCore/qentity.h>
#include <Qt3DRender/QRenderAspect>
#include <Qt3DRender/qframegraph.h>
#include <Qt3DRender/qrendersurfaceselector.h>
#include <Qt3DInput/QInputAspect>

#include <QtQuick/qquickwindow.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

/*!
    \class Qt3DCore::Scene3DItem
    \internal

    \brief The Qt3DCore::Scene3DItem class is a QQuickItem subclass used to integrate
    a Qt3D scene into a QtQuick 2 scene.

    The Qt3DCore::Scene3DItem class renders a Qt3D scene, provided by a Qt3DCore::QEntity
    into a multisampled Framebuffer object that is later blitted into a non
    multisampled Framebuffer object to be then renderer through the use of a
    Qt3DCore::Scene3DSGNode with premultiplied alpha.
 */
Scene3DItem::Scene3DItem(QQuickItem *parent)
    : QQuickItem(parent)
    , m_entity(Q_NULLPTR)
    , m_aspectEngine(new Qt3DCore::QAspectEngine())
    , m_renderAspect(new QRenderAspect(QRenderAspect::Synchronous))
    , m_renderer(Q_NULLPTR)
    , m_rendererCleaner(new Scene3DCleaner())
    , m_multisample(true)
{
    setFlag(QQuickItem::ItemHasContents, true);
    setAcceptedMouseButtons(Qt::MouseButtonMask);
    setAcceptHoverEvents(true);

    // We need to register the event source in the main thread
    QVariantMap data;
    data.insert(QStringLiteral("eventSource"), QVariant::fromValue(this));
    m_aspectEngine->setData(data);

    m_aspectEngine->registerAspect(m_renderAspect);
}

Scene3DItem::~Scene3DItem()
{
    // When the window is closed, it first destroys all of its children. At
    // this point, Scene3DItem is destroyed but the Renderer, AspectEngine and
    // Scene3DSGNode still exist and will perform their cleanup on their own.
}

QStringList Scene3DItem::aspects() const
{
    return m_aspects;
}

Qt3DCore::QEntity *Scene3DItem::entity() const
{
    return m_entity;
}

void Scene3DItem::setAspects(const QStringList &aspects)
{
    if (!m_aspects.isEmpty()) {
        qWarning() << "Aspects already set on the Scene3D, ignoring";
        return;
    }

    m_aspects = aspects;

    // Aspects are owned by the aspect engine
    Q_FOREACH (const QString &aspect, m_aspects) {
        if (aspect == QStringLiteral("render")) // This one is hardwired anyway
            continue;
        if (aspect == QStringLiteral("input"))  {
            m_aspectEngine->registerAspect(new Qt3DInput::QInputAspect);
            continue;
        }

        m_aspectEngine->registerAspect(aspect);
    }

    emit aspectsChanged();
}

void Scene3DItem::setEntity(Qt3DCore::QEntity *entity)
{
    if (entity == m_entity)
        return;

    m_entity = entity;
    emit entityChanged();
}

void Scene3DItem::applyRootEntityChange()
{
    if (m_aspectEngine->rootEntity() != m_entity) {
        m_aspectEngine->setRootEntity(m_entity);

        // Set the render surface
        if (m_entity)
            setWindowSurface(m_entity);
    }
}

void Scene3DItem::setWindowSurface(QObject *rootObject)
{
    // Find surface selector in framegraph and set ourselves up as the
    // render surface there
    Qt3DRender::QFrameGraph *frameGraphComponent
        = rootObject->findChild<Qt3DRender::QFrameGraph *>();
    if (!frameGraphComponent) {
        qWarning() << "No frame graph component found";
        return;
    }

    Qt3DCore::QNode *frameGraphRoot = frameGraphComponent->activeFrameGraph();
    if (!frameGraphRoot) {
        qWarning() << "No active frame graph found";
        return;
    }

    Qt3DRender::QRenderSurfaceSelector *surfaceSelector
        = frameGraphRoot->findChild<Qt3DRender::QRenderSurfaceSelector *>();
    if (!surfaceSelector) {
        qWarning() << "No render surface selector found in frame graph";
        return;
    }

    surfaceSelector->setWindow(this->window());
}

/*!
    \return \c true if a multisample renderbuffer is in use.
 */
bool Scene3DItem::multisample() const
{
    return m_multisample;
}

/*!
    Enables or disables the usage of multisample renderbuffers based on \a enable.

    By default multisampling is enabled. If the OpenGL implementation has no
    support for multisample renderbuffers or framebuffer blits, the request to
    use multisampling is ignored.

    \note Refrain from changing the value frequently as it involves expensive
    and potentially slow initialization of framebuffers and other OpenGL
    resources.
 */
void Scene3DItem::setMultisample(bool enable)
{
    if (m_multisample != enable) {
        m_multisample = enable;
        emit multisampleChanged();
        update();
    }
}

QSGNode *Scene3DItem::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *)
{
    // If the node already exists
    // we delete it and recreate it
    // as we need to resize the FBO
    if (node) {
        delete node;
        node = Q_NULLPTR;
    }

    if (m_renderer == Q_NULLPTR) {
        m_renderer = new Scene3DRenderer(this, m_aspectEngine, m_renderAspect);
        m_renderer->setCleanerHelper(m_rendererCleaner);
    }

    // The main thread is blocked, it is now time to sync data between the renderer and the item.
    m_renderer->synchronize();

    Scene3DSGNode *fboNode = new Scene3DSGNode();
    fboNode->setRect(boundingRect());
    m_renderer->setSGNode(fboNode);
    return fboNode;
}

} // namespace Qt3DRender

QT_END_NAMESPACE
