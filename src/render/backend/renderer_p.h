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

#ifndef QT3DRENDER_RENDER_RENDERER_H
#define QT3DRENDER_RENDER_RENDERER_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <Qt3DRender/qrenderaspect.h>
#include <Qt3DRender/qtechnique.h>
#include <Qt3DRender/private/quniformvalue_p.h>
#include <Qt3DRender/private/handle_types_p.h>
#include <Qt3DCore/qaspectjob.h>

#include <QHash>
#include <QMatrix4x4>
#include <QObject>

#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMutex>
#include <QWaitCondition>
#include <QAtomicInt>
#include <QScopedPointer>
#include <QSemaphore>
#include <QThreadStorage>

QT_BEGIN_NAMESPACE

class QSurface;
class QOpenGLDebugLogger;
class QMouseEvent;

namespace Qt3DCore {
class QEntity;
class QFrameAllocator;
class QEventFilterService;
}

namespace Qt3DRender {

class QCamera;
class QMaterial;
class QShaderProgram;
class QMesh;
class QRenderPass;
class QAbstractShapeMesh;
class QGraphicsApiFilter;
class QAbstractSceneParser;

namespace Render {

class CameraLens;
class GraphicsContext;
class FrameGraphNode;
class Material;
class Technique;
class Shader;
class Entity;
class RenderCommand;
class RenderQueue;
class RenderView;
class Effect;
class RenderPass;
class RenderThread;
class RenderStateSet;
class VSyncFrameAdvanceService;
class PickEventFilter;
class NodeManagers;

class Q_AUTOTEST_EXPORT Renderer
{
public:
    explicit Renderer(QRenderAspect::RenderType type);
    ~Renderer();

    void setQRenderAspect(QRenderAspect *aspect) { m_rendererAspect = aspect; }
    QRenderAspect *rendererAspect() const { return m_rendererAspect; }

    NodeManagers *nodeManagers() const;

    void createAllocators(Qt3DCore::QAbstractAspectJobManager *jobManager);
    void destroyAllocators(Qt3DCore::QAbstractAspectJobManager *jobManager);

    Qt3DCore::QFrameAllocator *currentFrameAllocator();

    QThreadStorage<Qt3DCore::QFrameAllocator *> *tlsAllocators();

    void setFrameGraphRoot(const Qt3DCore::QNodeId &fgRoot);
    Render::FrameGraphNode *frameGraphRoot() const;

    void setSceneGraphRoot(Entity *sgRoot);
    Entity *renderSceneRoot() const { return m_renderSceneRoot; }

    void render();
    void doRender();

    QVector<Qt3DCore::QAspectJobPtr> createRenderBinJobs();
    QVector<Qt3DCore::QAspectJobPtr> createRenderBufferJobs();
    QVector<Qt3DCore::QAspectJobPtr> createGeometryRendererJobs();
    Qt3DCore::QAspectJobPtr createRenderViewJob(FrameGraphNode *node, int submitOrderIndex);
    void executeCommands(const QVector<RenderCommand *> &commands);
    Attribute *updateBuffersAndAttributes(Geometry *geometry, RenderCommand *command, GLsizei &count, bool forceUpdate);
    void addAllocator(Qt3DCore::QFrameAllocator *allocator);


    inline HMaterial defaultMaterialHandle() const { return m_defaultMaterialHandle; }
    inline HEffect defaultEffectHandle() const { return m_defaultEffectHandle; }
    inline HTechnique defaultTechniqueHandle() const { return m_defaultTechniqueHandle; }
    inline HRenderPass defaultRenderPassHandle() const { return m_defaultRenderPassHandle; }
    inline RenderStateSet *defaultRenderState() const { return m_defaultRenderStateSet; }

    inline QList<QAbstractSceneParser *> sceneParsers() const { return m_sceneParsers; }
    inline VSyncFrameAdvanceService *vsyncFrameAdvanceService() const { return m_vsyncFrameAdvanceService.data(); }

    QList<QMouseEvent> pendingPickingEvents() const;

    QGraphicsApiFilter *contextInfo() const;

    void setSurface(QSurface *s);
    inline QSurface *surface() const { return m_surface; }
    void registerEventFilter(Qt3DCore::QEventFilterService *service);

    void enqueueRenderView(RenderView *renderView, int submitOrder);
    bool submitRenderViews();

    void initialize(QOpenGLContext *context = Q_NULLPTR);
    void shutdown();

    QMutex* mutex() { return &m_mutex; }
    bool isRunning() const { return m_running.load(); }

#ifdef QT3D_RENDER_UNIT_TESTS
public:
#else
private:
#endif
    bool canRender() const;

    QRenderAspect *m_rendererAspect;
    NodeManagers *m_nodesManager;

    // Frame graph root
    Qt3DCore::QNodeId m_frameGraphRootUuid;

    Entity *m_renderSceneRoot;

    QHash<QMaterial*, Material*> m_materialHash;
    QHash<QTechnique *, Technique*> m_techniqueHash;
    QHash<QShaderProgram*, Shader*> m_shaderHash;

    QMaterial* m_defaultMaterial;
    QTechnique* m_defaultTechnique;

    HMaterial m_defaultMaterialHandle;
    HEffect m_defaultEffectHandle;
    HTechnique m_defaultTechniqueHandle;
    HRenderPass m_defaultRenderPassHandle;

    // Fail safe values that we can use if a RenderCommand
    // is missing a shader
    Shader *m_defaultRenderShader;
    RenderStateSet *m_defaultRenderStateSet;
    QHash<QString, QString> m_defaultParameterToGLSLAttributeNames;
    QUniformPack m_defaultUniformPack;

    QScopedPointer<GraphicsContext> m_graphicsContext;
    QSurface *m_surface;
    QObject *m_eventSource;


    RenderQueue *m_renderQueue;
    QScopedPointer<RenderThread> m_renderThread;
    QScopedPointer<VSyncFrameAdvanceService> m_vsyncFrameAdvanceService;

    void buildDefaultMaterial();
    void buildDefaultTechnique();
    void loadSceneParsers();

    QMutex m_mutex;
    QSemaphore m_submitRenderViewsSemaphore;
    QWaitCondition m_waitForWindowToBeSetCondition;
    QWaitCondition m_waitForInitializationToBeCompleted;

    static void createThreadLocalAllocator(void *renderer);
    static void destroyThreadLocalAllocator(void *renderer);
    QThreadStorage<Qt3DCore::QFrameAllocator *> m_tlsAllocators;

    QAtomicInt m_running;

    QScopedPointer<QOpenGLDebugLogger> m_debugLogger;
    QScopedPointer<PickEventFilter> m_pickEventFilter;
    QList<QAbstractSceneParser *> m_sceneParsers;
    QVector<Qt3DCore::QFrameAllocator *> m_allocators;

    QVector<Attribute *> m_dirtyAttributes;
    QVector<Geometry *> m_dirtyGeometry;
};

} // namespace Render
} // namespace Qt3DRender

QT_END_NAMESPACE

#endif // QT3DRENDER_RENDER_RENDERER_H
