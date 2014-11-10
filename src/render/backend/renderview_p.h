/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
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

#ifndef QT3D_RENDER_RENDERVIEW_H
#define QT3D_RENDER_RENDERVIEW_H

#include <Qt3DRenderer/qparameter.h>
#include <Qt3DRenderer/qclearbuffer.h>
#include <Qt3DRenderer/private/renderer_p.h>
#include <Qt3DRenderer/private/rendercameralens_p.h>
#include <Qt3DRenderer/private/attachmentpack_p.h>
#include <Qt3DRenderer/private/handle_types_p.h>
#include <Qt3DRenderer/qparameter.h>

#include <Qt3DCore/qframeallocator.h>

#include <QVector>
#include <QMutex>
#include <QColor>

QT_BEGIN_NAMESPACE

namespace Qt3D {

class QRenderPass;
class QFrameAllocator;

namespace Render {

class Renderer;
class RenderCommand;
class RenderPassFilter;
class TechniqueFilter;
class ViewportNode;
class RenderEffect;
class RenderRenderPass;

typedef QPair<HLight, QMatrix4x4> LightPair;

// This class is kind of analogous to RenderBin but I want to avoid trampling
// on that until we get this working
class Q_AUTOTEST_EXPORT RenderView
{
public:
    RenderView();
    ~RenderView();

    static void operator delete(void *ptr);
    static void operator delete(void *ptr, void *);

    // TODO: Add a way to specify a sort predicate for the RenderCommands
    void sort();

    inline void setRenderer(Renderer *renderer) { m_renderer = renderer; }
    inline Renderer *renderer() const { return m_renderer; }

    inline void setAllocator(QFrameAllocator *allocator)
    {
        m_allocator = allocator;
        m_data = m_allocator->allocate<InnerData>();
        // If we have a viewMatrix pointer instead of directly a QMatrix4x4 object in RenderView
        // This allows us to keep the size of RenderView smaller and avoid huge block fragmentation
        //
        // TODO: Is this worth it here. We don't have that many RenderViews to iterate over. This
        // level of memory management would be better in RenderEntity's matrices as they will
        // help cache performance during iteration
        m_data->m_viewMatrix = m_allocator->allocate<QMatrix4x4>();
    }
    inline QFrameAllocator *allocator() const { return m_allocator; }

    inline void setRenderCamera(RenderCameraLens *renderCamera) { m_data->m_renderCamera = renderCamera; }
    inline RenderCameraLens *renderCamera() const { return m_data->m_renderCamera; }

    inline void setViewMatrix(const QMatrix4x4 viewMatrix) { *(m_data->m_viewMatrix) = viewMatrix; }
    inline QMatrix4x4 viewmatrix() const { Q_ASSERT(m_data->m_viewMatrix); return *(m_data->m_viewMatrix); }

    inline void setEyePosition(const QVector3D &eyePos) { m_data->m_eyePos = eyePos; }
    inline QVector3D eyePosition() const { return m_data->m_eyePos; }

    inline void appendLayerFilter(const QStringList &layers) { m_data->m_layers << layers; }
    inline QStringList layerFilters() const { return m_data->m_layers; }

    inline void setRenderPassFilter(const RenderPassFilter *rpFilter) { m_data->m_passFilter = rpFilter; }
    inline const RenderPassFilter *renderPassFilter() const { return m_data->m_passFilter; }

    inline void setTechniqueFilter(const TechniqueFilter *filter) { m_data->m_techniqueFilter = filter; }
    inline const TechniqueFilter *techniqueFilter() const { return m_data->m_techniqueFilter; }

    // TODO: Get rid of this overly complex memory management by splitting out the
    // InnerData as a RenderViewConfig struct. This can be created by setRenderViewConfigFromFrameGraphLeafNode
    // and passed along with the RenderView to the functions that populate the renderview
    inline void setViewport(const QRectF &vp)
    {
        if (!m_viewport) {
            Q_ASSERT(m_allocator);
            m_viewport = m_allocator->allocate<QRectF>();
            *m_viewport = QRectF(0.0f, 0.0f, 1.0f, 1.0f);
        }
        *m_viewport = vp;
    }
    inline QRectF viewport() const
    {
        if (!m_viewport) {
            Q_ASSERT(m_allocator);
            m_viewport = m_allocator->allocate<QRectF>();
            *m_viewport = QRectF(0.0f, 0.0f, 1.0f, 1.0f);
        }
        return *m_viewport;
    }

    inline void setClearColor(const QColor &c)
    {
        if (!m_clearColor) {
            Q_ASSERT(m_allocator);
            m_clearColor = m_allocator->allocate<QColor>();
            *m_clearColor = QColor(QColor::Invalid);
        }
        *m_clearColor = c;
    }
    inline QColor clearColor() const
    {
        if (!m_clearColor) {
            Q_ASSERT(m_allocator);
            m_clearColor = m_allocator->allocate<QColor>();
            *m_clearColor = QColor(QColor::Invalid);
        }
        return *m_clearColor;
    }

    inline void setClearBuffer(QClearBuffer::BufferType clearBuffer) { m_clearBuffer = clearBuffer; }
    inline QClearBuffer::BufferType clearBuffer() const { return m_clearBuffer; }

    inline void appendLight(HLight lightHandle, const QMatrix4x4 &worldMatrix)
    {
        m_data->m_lights.append(LightPair(lightHandle, worldMatrix));
    }

    void buildRenderCommands(RenderEntity *preprocessedTreeRoot);
    QVector<RenderCommand *> commands() const { return m_commands; }


    // This can be removed once we're 100% sure the QFrameAllocator is death proof
    void setFrameIndex(int index) { m_frameIndex = index; }
    int frameIndex() const { return m_frameIndex; }

    void addRenderAttachment(Attachment attachment) { m_attachmentPack.addAttachment(attachment); }
    const AttachmentPack &attachmentPack() const;

    void setRenderTargetHandle(HTarget renderTargetHandle) { m_renderTarget = renderTargetHandle; }
    HTarget renderTargetHandle() const { return m_renderTarget; }

    void addSortCriteria(const QList<QNodeUuid> &sortMethodUid) { m_data->m_sortingCriteria.append(sortMethodUid); }

private:
    void setShaderAndUniforms(RenderCommand *command, RenderRenderPass *pass, QHash<QString, QVariant> &parameters, const QMatrix4x4 &worldTransform);

    Renderer *m_renderer;
    QFrameAllocator *m_allocator;

    // Helps making the size of RenderView smaller
    // Contains all the data needed for the actual building of the RenderView
    // But that aren't used later by the Renderer
    struct InnerData {
        InnerData()
            : m_renderCamera(Q_NULLPTR)
            , m_techniqueFilter(Q_NULLPTR)
            , m_passFilter(Q_NULLPTR)
            , m_viewMatrix(Q_NULLPTR)
        {
        }
        RenderCameraLens *m_renderCamera;
        const TechniqueFilter *m_techniqueFilter;
        const RenderPassFilter *m_passFilter;
        QMatrix4x4 *m_viewMatrix;
        QStringList m_layers;
        QList<LightPair> m_lights;
        QList<QNodeUuid> m_sortingCriteria;
        QVector3D m_eyePos;
    } *m_data;

    mutable QColor *m_clearColor;
    mutable QRectF *m_viewport;
    HTarget m_renderTarget;
    AttachmentPack m_attachmentPack;
    QClearBuffer::BufferType m_clearBuffer;
    int m_frameIndex;

    // We do not use pointers to RenderNodes or Drawable's here so that the
    // render aspect is free to change the drawables on the next frame whilst
    // the render thread is submitting these commands.
    QVector<RenderCommand *> m_commands;

    typedef QHash<QString, QUniformValue* (RenderView::*)(const QMatrix4x4& model) const> StandardUniformsPFuncsHash;
    static StandardUniformsPFuncsHash ms_standardUniformSetters;
    static StandardUniformsPFuncsHash initializeStandardUniformSetters();
    static QStringList ms_standardAttributesNames;
    static QStringList initializeStandardAttributeNames();

    QUniformValue *modelMatrix(const QMatrix4x4& model) const;
    QUniformValue *viewMatrix(const QMatrix4x4&) const;
    QUniformValue *projectionMatrix(const QMatrix4x4 &) const;
    QUniformValue *modelViewMatrix(const QMatrix4x4 &model) const;
    QUniformValue *modelViewProjectionMatrix(const QMatrix4x4 &model) const;
    QUniformValue *inverseModelMatrix(const QMatrix4x4 &model) const;
    QUniformValue *inverseViewMatrix(const QMatrix4x4 &) const;
    QUniformValue *inverseProjectionMatrix(const QMatrix4x4 &) const;
    QUniformValue *inverseModelViewMatrix(const QMatrix4x4 &model) const;
    QUniformValue *inverseModelViewProjectionMatrix(const QMatrix4x4 &model) const;
    QUniformValue *modelNormalMatrix(const QMatrix4x4 &model) const;
    QUniformValue *modelViewNormalMatrix(const QMatrix4x4 &model) const;
    QUniformValue *viewportMatrix(const QMatrix4x4 &model) const;
    QUniformValue *inverseViewportMatrix(const QMatrix4x4 &model) const;

    void setUniformValue(QUniformPack &uniformPack, const QString &name, const QVariant &value);
    void buildSortingKey(RenderCommand *command);
};

} // namespace Render
} // namespace Qt3D

QT_END_NAMESPACE

#endif // QT3D_RENDER_RENDERVIEW_H
