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

#include "renderview_p.h"
#include <Qt3DRenderer/qmaterial.h>
#include <Qt3DRenderer/qopenglfilter.h>
#include <Qt3DRenderer/renderlogging.h>
#include <Qt3DRenderer/qtexture.h>
#include <Qt3DRenderer/qrendertarget.h>

#include <Qt3DRenderer/private/cameramanager_p.h>
#include <Qt3DRenderer/private/cameraselectornode_p.h>
#include <Qt3DRenderer/private/criterionmanager_p.h>
#include <Qt3DRenderer/private/effectmanager_p.h>
#include <Qt3DRenderer/private/entitymanager_p.h>
#include <Qt3DRenderer/private/framegraphnode_p.h>
#include <Qt3DRenderer/private/layerfilternode_p.h>
#include <Qt3DRenderer/private/layermanager_p.h>
#include <Qt3DRenderer/private/lightmanager_p.h>
#include <Qt3DRenderer/private/materialmanager_p.h>
#include <Qt3DRenderer/private/qmeshdata_p.h>
#include <Qt3DRenderer/private/meshdatamanager_p.h>
#include <Qt3DRenderer/private/meshmanager_p.h>
#include <Qt3DRenderer/private/qparameter_p.h>
#include <Qt3DRenderer/private/qparametermapper_p.h>
#include <Qt3DRenderer/private/rendercameralens_p.h>
#include <Qt3DRenderer/private/rendercommand_p.h>
#include <Qt3DRenderer/private/rendereffect_p.h>
#include <Qt3DRenderer/private/renderentity_p.h>
#include <Qt3DRenderer/private/renderer_p.h>
#include <Qt3DRenderer/private/renderlayer_p.h>
#include <Qt3DRenderer/private/renderlight_p.h>
#include <Qt3DRenderer/private/renderpassfilternode_p.h>
#include <Qt3DRenderer/private/renderpassmanager_p.h>
#include <Qt3DRenderer/private/renderrenderpass_p.h>
#include <Qt3DRenderer/private/shadermanager_p.h>
#include <Qt3DRenderer/private/techniquefilternode_p.h>
#include <Qt3DRenderer/private/techniquemanager_p.h>
#include <Qt3DRenderer/private/texturemanager_p.h>
#include <Qt3DRenderer/private/vaomanager_p.h>
#include <Qt3DRenderer/private/viewportnode_p.h>
#include <Qt3DRenderer/private/rendertargetselectornode_p.h>
#include <Qt3DRenderer/private/rendertargetmanager_p.h>
#include <Qt3DRenderer/private/attachmentmanager_p.h>
#include <Qt3DRenderer/private/clearbuffer_p.h>

#include "qalphatest.h"
#include "qblendequation.h"
#include "qblendstate.h"
#include "qcullface.h"
#include "qdepthmask.h"
#include "qdepthtest.h"
#include "qdithering.h"
#include "qfrontface.h"
#include "qscissortest.h"
#include "qstenciltest.h"
#include <Qt3DRenderer/private/blendstate_p.h>

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qabstracteffect.h>
#include <Qt3DCore/qabstracttechnique.h>
#include <Qt3DCore/qframeallocator.h>

#include <algorithm>

#include <QDebug>

QT_BEGIN_NAMESPACE

namespace Qt3D {
namespace Render {

RenderView::standardUniformsPFuncsHash RenderView::m_standardUniformSetters = RenderView::initializeStandardUniformSetters();
QStringList RenderView::m_standardAttributesNames = RenderView::initializeStandardAttributeNames();

RenderView::standardUniformsPFuncsHash RenderView::initializeStandardUniformSetters()
{
    RenderView::standardUniformsPFuncsHash setters;

    setters[QStringLiteral("modelMatrix")] = &RenderView::modelMatrix;
    setters[QStringLiteral("viewMatrix")] = &RenderView::viewMatrix;
    setters[QStringLiteral("projectionMatrix")] = &RenderView::projectionMatrix;
    setters[QStringLiteral("modelView")] = &RenderView::modelViewMatrix;
    setters[QStringLiteral("modelViewProjection")] = &RenderView::modelViewProjectionMatrix;
    setters[QStringLiteral("mvp")] = &RenderView::modelViewProjectionMatrix;
    setters[QStringLiteral("inverseModelMatrix")] = &RenderView::inverseModelMatrix;
    setters[QStringLiteral("inverViewMatrix")] = &RenderView::inverseViewMatrix;
    setters[QStringLiteral("inverseProjectionMatrix")] = &RenderView::inverseProjectionMatrix;
    setters[QStringLiteral("inverseModelView")] = &RenderView::inverseModelViewMatrix;
    setters[QStringLiteral("inverseModelViewProjection")] = &RenderView::inverseModelViewProjectionMatrix;
    setters[QStringLiteral("modelNormalMatrix")] = &RenderView::modelNormalMatrix;
    setters[QStringLiteral("modelViewNormal")] = &RenderView::modelViewNormalMatrix;

    return setters;
}

QStringList RenderView::initializeStandardAttributeNames()
{
    QStringList attributesNames;

    attributesNames << QAbstractMeshData::defaultPositionAttributeName();
    attributesNames << QAbstractMeshData::defaultTextureCoordinateAttributeName();
    attributesNames << QAbstractMeshData::defaultNormalAttributeName();
    attributesNames << QAbstractMeshData::defaultColorAttributeName();
    attributesNames << QAbstractMeshData::defaultTangentAttributeName();

    return attributesNames;
}

QUniformValue *RenderView::modelMatrix(const QMatrix4x4 &model) const
{
    SpecifiedUniform<QMatrix4x4> *t = m_allocator->allocate<SpecifiedUniform<QMatrix4x4> >();
    t->setValue(model);
    return t;
}

QUniformValue *RenderView::viewMatrix(const QMatrix4x4 &) const
{
    SpecifiedUniform<QMatrix4x4> *t = m_allocator->allocate<SpecifiedUniform<QMatrix4x4> >();
    t->setValue(*m_viewMatrix);
    return t;
}

QUniformValue *RenderView::projectionMatrix(const QMatrix4x4 &) const
{
    SpecifiedUniform<QMatrix4x4> *t = m_allocator->allocate<SpecifiedUniform<QMatrix4x4> >();
    t->setValue(m_renderCamera->projection());
    return t;
}

QUniformValue *RenderView::modelViewMatrix(const QMatrix4x4 &model) const
{
    SpecifiedUniform<QMatrix4x4> *t = m_allocator->allocate<SpecifiedUniform<QMatrix4x4> >();
    t->setValue(*m_viewMatrix * model);
    return t;
}

QUniformValue *RenderView::modelViewProjectionMatrix(const QMatrix4x4 &model) const
{
    SpecifiedUniform<QMatrix4x4> *t = m_allocator->allocate<SpecifiedUniform<QMatrix4x4> >();
    QMatrix4x4 projection;
    if (m_renderCamera)
        projection = m_renderCamera->projection();
    t->setValue(projection * *m_viewMatrix * model);
    return t;
}

QUniformValue *RenderView::inverseModelMatrix(const QMatrix4x4 &model) const
{
    SpecifiedUniform<QMatrix4x4> *t = m_allocator->allocate<SpecifiedUniform<QMatrix4x4> >();
    t->setValue(model.inverted());
    return t;
}

QUniformValue *RenderView::inverseViewMatrix(const QMatrix4x4 &) const
{
    SpecifiedUniform<QMatrix4x4> *t = m_allocator->allocate<SpecifiedUniform<QMatrix4x4> >();
    t->setValue(m_viewMatrix->inverted());
    return t;
}

QUniformValue *RenderView::inverseProjectionMatrix(const QMatrix4x4 &) const
{
    SpecifiedUniform<QMatrix4x4> *t = m_allocator->allocate<SpecifiedUniform<QMatrix4x4> >();
    QMatrix4x4 projection;
    if (m_renderCamera)
        projection = m_renderCamera->projection();
    t->setValue(projection.inverted());
    return t;
}

QUniformValue *RenderView::inverseModelViewMatrix(const QMatrix4x4 &model) const
{
    SpecifiedUniform<QMatrix4x4> *t = m_allocator->allocate<SpecifiedUniform<QMatrix4x4> >();
    t->setValue((*m_viewMatrix * model).inverted());
    return t;
}

QUniformValue *RenderView::inverseModelViewProjectionMatrix(const QMatrix4x4 &model) const
{
    SpecifiedUniform<QMatrix4x4> *t = m_allocator->allocate<SpecifiedUniform<QMatrix4x4> >();
    QMatrix4x4 projection;
    if (m_renderCamera)
        projection = m_renderCamera->projection();
    t->setValue((projection * *m_viewMatrix * model).inverted());
    return t;
}

QUniformValue *RenderView::modelNormalMatrix(const QMatrix4x4 &model) const
{
    SpecifiedUniform<QMatrix3x3> *t = m_allocator->allocate<SpecifiedUniform<QMatrix3x3> >();
    t->setValue(model.normalMatrix());
    return t;
}

QUniformValue *RenderView::modelViewNormalMatrix(const QMatrix4x4 &model) const
{
    SpecifiedUniform<QMatrix3x3> *t = m_allocator->allocate<SpecifiedUniform<QMatrix3x3> >();
    t->setValue((*m_viewMatrix * model).normalMatrix());
    return t;
}

RenderView::RenderView()
    : m_renderer(Q_NULLPTR)
    , m_allocator(Q_NULLPTR)
    , m_renderCamera(Q_NULLPTR)
    , m_techniqueFilter(Q_NULLPTR)
    , m_passFilter(Q_NULLPTR)
    , m_viewport(Q_NULLPTR)
    , m_viewMatrix(Q_NULLPTR)
    , m_clearColor(Q_NULLPTR)
{
}

RenderView::~RenderView()
{
    if (m_allocator == Q_NULLPTR) // Mainly needed for unit tests
        return ;
    Q_FOREACH (RenderCommand *command, m_commands) {
        // Deallocate all uniform values of the QUniformPack of each RenderCommand
        Q_FOREACH (const QUniformValue *v, command->m_uniforms.uniforms().values())
            m_allocator->deallocate<QUniformValue>(const_cast<QUniformValue *>(v));
        if (command->m_stateSet != Q_NULLPTR) // We do not delete the RenderState as there are stored statically
            m_allocator->deallocate<RenderStateSet>(command->m_stateSet);
        // Deallocate RenderCommand
        m_allocator->deallocate<RenderCommand>(command);
    }
    // Deallocate viewMatrix
    m_allocator->deallocate<QMatrix4x4>(m_viewMatrix);
    // Deallocate viewport rect
    m_allocator->deallocate<QRectF>(m_viewport);
    // Deallocate clearColor
    m_allocator->deallocate<QColor>(m_clearColor);
}

// We need to overload the delete operator so that when the Renderer deletes the list of RenderViews, each RenderView
// can clear itself with the frame allocator and deletes its RenderViews
void RenderView::operator delete(void *ptr)
{
    RenderView *rView = static_cast<RenderView *>(ptr);
    if (rView != Q_NULLPTR && rView->m_allocator != Q_NULLPTR)
        rView->m_allocator->deallocateRawMemory<RenderView>(rView);
}

// Since placement new is used we need a matching operator delete, at least MSVC complains otherwise
void RenderView::operator delete(void *ptr, void *)
{
    RenderView *rView = static_cast<RenderView *>(ptr);
    if (rView != Q_NULLPTR && rView->m_allocator != Q_NULLPTR)
        rView->m_allocator->deallocateRawMemory<RenderView>(rView);
}

void RenderView::setConfigFromFrameGraphLeafNode(FrameGraphNode *fgLeaf)
{
    // The specific RenderPass to be used is also dependent upon the Effect and TechniqueFilter
    // which is referenced by the Material which is referenced by the RenderMesh. So we can
    // only store the filter info in the RenderView structure and use it to do the resolving
    // when we build the RenderCommand list.
    FrameGraphNode *node = fgLeaf;
    while (node != Q_NULLPTR) {
        FrameGraphNode::FrameGraphNodeType type = node->nodeType();
        switch (type) {
        case FrameGraphNode::CameraSelector: // Can be set once
            if (m_renderCamera == Q_NULLPTR) {
                CameraSelector *cameraSelector = static_cast<CameraSelector *>(node);
                RenderEntity *tmpCamNode = m_renderer->renderNodesManager()->lookupResource(cameraSelector->cameraUuid());
                if (tmpCamNode) {
                    m_renderCamera = tmpCamNode->renderComponent<RenderCameraLens>();
                    // If we have a viewMatrix pointer instead of directly a QMatrix4x4 object in RenderView
                    // This allows us to keep the size of RenderView smaller and avoid huge block fragmentation
                    *m_viewMatrix = *tmpCamNode->worldTransform();
                }
                break;
            }

        case FrameGraphNode::LayerFilter: // Can be set multiple times in the tree
            m_layers << static_cast<LayerFilterNode *>(node)->layers();
            break;

        case FrameGraphNode::RenderPassFilter:
            if (m_passFilter == Q_NULLPTR) // Can be set once
                m_passFilter = static_cast<RenderPassFilter *>(node);
            break;

        case FrameGraphNode::RenderTarget:
            if (m_renderTarget.isNull()) // Can be set once
                m_renderTarget = m_renderer->renderTargetManager()->lookupHandle(static_cast<RenderTargetSelector *>(node)->renderTargetUuid());
            RenderTarget *renderTarget;
            if ((renderTarget = m_renderer->renderTargetManager()->data(m_renderTarget)) != Q_NULLPTR) {
                // Add renderTarget Handle and build renderCommand AttachmentPack
                Q_FOREACH (const QUuid &attachmentId, renderTarget->renderAttachments()) {
                    RenderAttachment *attachment = m_renderer->attachmentManager()->lookupResource(attachmentId);
                    if (attachment != Q_NULLPTR)
                        m_attachmentPack.addAttachment(attachment->attachment());
                }
            }
            break;

        case FrameGraphNode::ClearBuffer:
            m_clearBuffer = static_cast<ClearBuffer *>(node)->type();
            break;

        case FrameGraphNode::TechniqueFilter:
            if (m_techniqueFilter == Q_NULLPTR) // Can be set once
                m_techniqueFilter = static_cast<TechniqueFilter *>(node);
            break;

        case FrameGraphNode::Viewport: // Can be set multiple times in the tree
            // If the Viewport has already been set in a lower node
            // Make it so that the new viewport is actually
            // a subregion relative to that of the parent viewport
            computeViewport(static_cast<ViewportNode *>(node));
            break;

        default:
            // Should never get here
            qCWarning(Backend) << "Unhandled FrameGraphNode type";
        }

        node = node->parent();
    }
}

void RenderView::sort()
{
    // TODO: Implement me!
    // Find a way to let user specify the sorting to perform
    // Maybe a Sorting tree within the FrameGraph or some predicates

    // The goal here is to sort RenderCommand by :
    // 1) Shader
    // 2) RenderStateSet
    // 2) Texture

    std::sort(m_commands.begin(), m_commands.end());

}

void RenderView::setRenderer(Renderer *renderer)
{
    m_renderer = renderer;
}

void RenderView::setAllocator(QFrameAllocator *allocator)
{
    m_allocator = allocator;
    m_viewMatrix = m_allocator->allocate<QMatrix4x4>();
}

void RenderView::preprocessRenderTree(RenderEntity *node)
{
    // Retrieve light for the currentNode and append it to list of current lights
    // As only light components of an Entity are considered active
    // Note : Layer filtering isn't applied there
    HLight lightHandle = node->componentHandle<RenderLight, 16>();
    if (!lightHandle.isNull())
        m_lights.append(LightPair(lightHandle, *node->worldTransform()));

    // Traverse children
    Q_FOREACH (RenderEntity *child, node->children())
        preprocessRenderTree(child);
}

// Tries to order renderCommand by shader so as to minimize shader changes
void RenderView::buildRenderCommands(RenderEntity *node)
{
    // Build renderCommand for current node
    if (checkContainedWithinLayer(node)) {
        RenderMesh *mesh = Q_NULLPTR;
        if (node->componentHandle<RenderMesh, 16>() != HMesh()
                && (mesh = node->renderComponent<RenderMesh>()) != Q_NULLPTR) {
            if (!mesh->meshData().isNull())
            {
                // Perform culling here
                // As shaders may be deforming, transforming the mesh
                // We might want to make that optional or dependent on an explicit bounding box item

                RenderMaterial *material = Q_NULLPTR;
                RenderEffect *effect = Q_NULLPTR;
                if ((material = node->renderComponent<RenderMaterial>()) != Q_NULLPTR)
                    effect = material->effect();
                RenderTechnique *technique = findTechniqueForEffect(effect);
                QList<RenderRenderPass *> passes = findRenderPassesForTechnique(technique);
                if (passes.isEmpty()) {
                    material = m_renderer->materialManager()->data(m_renderer->defaultMaterialHandle());
                    effect = m_renderer->effectManager()->data(m_renderer->defaultEffectHandle());
                    technique = m_renderer->techniqueManager()->data(m_renderer->defaultTechniqueHandle());
                    passes << m_renderer->renderPassManager()->data(m_renderer->defaultRenderPassHandle());
                }
                QHash<QString, QVariant> parameters = parametersFromMaterialEffectTechnique(material, effect, technique);

                // 1 RenderCommand per RenderPass pass on an Entity with a Mesh
                Q_FOREACH (RenderRenderPass *pass, passes) {
                    RenderCommand *command = m_allocator->allocate<RenderCommand>();
                    command->m_meshData = mesh->meshData();
                    command->m_instancesCount = 0;
                    command->m_stateSet = buildRenderStateSet(pass);
                    setShaderAndUniforms(command, pass, parameters, *(node->worldTransform()));
                    m_commands.append(command);
                }
            }
        }
    }

    // Traverse children
    Q_FOREACH (RenderEntity *child, node->children())
        buildRenderCommands(child);
}

const AttachmentPack &RenderView::attachmentPack() const
{
    return m_attachmentPack;
}

HTarget RenderView::renderTarget() const
{
    return m_renderTarget;
}

RenderTechnique *RenderView::findTechniqueForEffect(RenderEffect *effect)
{
    if (effect != Q_NULLPTR) {
        Q_FOREACH (const QUuid &techniqueId, effect->techniques()) {
            RenderTechnique *technique = m_renderer->techniqueManager()->lookupResource(techniqueId);
            if (technique != Q_NULLPTR &&
                    *m_renderer->contextInfo() == *technique->openGLFilter()) {
                // If no techniqueFilter is present, we return the technique as it satisfies OpenGL version
                bool findMatch = (m_techniqueFilter == Q_NULLPTR || m_techniqueFilter->filters().size() == 0) ? true : false;
                if (!findMatch && technique->criteria().size() >= m_techniqueFilter->filters().size()) {
                    Q_FOREACH (const QUuid &refCritId, m_techniqueFilter->filters()) {
                        RenderCriterion *refCriterion = m_renderer->criterionManager()->lookupResource(refCritId);
                        findMatch = false;
                        Q_FOREACH (const QUuid &critId, technique->criteria()) {
                            RenderCriterion *rCrit = m_renderer->criterionManager()->lookupResource(critId);
                            if ((findMatch = (*rCrit == *refCriterion)))
                                break;
                        }
                        if (!findMatch) // No match for TechniqueFilter criterion in any of the technique's criteria
                            break;
                    }
                }
                if (findMatch) // If all criteria matched
                    return technique;
            }
        }
    }
    return Q_NULLPTR;
}

QList<RenderRenderPass *> RenderView::findRenderPassesForTechnique(RenderTechnique *technique)
{
    QList<RenderRenderPass *> passes;
    if (technique != Q_NULLPTR) {
        Q_FOREACH (const QUuid &passId, technique->renderPasses()) {
            RenderRenderPass *renderPass = m_renderer->renderPassManager()->lookupResource(passId);

            if (renderPass != Q_NULLPTR) {
                bool findMatch = (m_passFilter == Q_NULLPTR || m_passFilter->filters().size() == 0) ? true : false;
                if (!findMatch && renderPass->criteria().size() >= m_passFilter->filters().size())
                {
                    Q_FOREACH (const QUuid &refCritId, m_passFilter->filters()) {
                        RenderCriterion *refCriterion = m_renderer->criterionManager()->lookupResource(refCritId);
                        findMatch = false;
                        Q_FOREACH (const QUuid &critId, renderPass->criteria()) {
                            RenderCriterion *rCrit = m_renderer->criterionManager()->lookupResource(critId);
                            if ((findMatch = (*rCrit == *refCriterion)))
                                break;
                        }
                        if (!findMatch) // No match for TechniqueFilter criterion in any of the technique's criteria
                            break;
                    }
                }
                if (findMatch)
                    passes << renderPass;
            }
        }
    }
    return passes;
}

QHash<QString, QVariant> RenderView::parametersFromMaterialEffectTechnique(RenderMaterial *material,
                                                                           RenderEffect *effect,
                                                                           RenderTechnique *technique)
{
    QHash<QString, QVariant> params;

    // Material is preferred over Effect
    // Effect is preferred over Technique
    // By filling the hash in reverse preference order, we're ensured that we preserve preference

    if (effect != Q_NULLPTR)
        Q_FOREACH (const QString &key, effect->parameters().keys())
            params[key] = effect->parameters()[key];

    if (technique != Q_NULLPTR)
        Q_FOREACH (const QString &key, technique->parameters().keys())
            params[key] = technique->parameters()[key];

    if (material != Q_NULLPTR)
        Q_FOREACH (const QString &key, material->parameters().keys())
            params[key] = material->parameters()[key];

    return params;
}

// Build a RenderStateSet from the QRenderState stored in the RenderRenderPass
RenderStateSet *RenderView::buildRenderStateSet(RenderRenderPass *pass)
{
    if (pass != Q_NULLPTR && pass->renderStates().count() > 0) {
        RenderStateSet *stateSet = m_allocator->allocate<RenderStateSet>();

        Q_FOREACH (QRenderState *renderState, pass->renderStates()) {
            if (qobject_cast<QAlphaTest *>(renderState) != Q_NULLPTR) {
                QAlphaTest *alphaTest = qobject_cast<QAlphaTest *>(renderState);
                stateSet->addState(AlphaFunc::getOrCreate(alphaTest->func(), alphaTest->clamp()));
            }
            else if (qobject_cast<QBlendEquation *>(renderState) != Q_NULLPTR) {
                QBlendEquation *blendEquation = qobject_cast<QBlendEquation *>(renderState);
                stateSet->addState(BlendEquation::getOrCreate(blendEquation->mode()));
            }
            else if (qobject_cast<QBlendState *>(renderState) != Q_NULLPTR) {
                QBlendState *blendState = qobject_cast<QBlendState *>(renderState);
                // TO DO : Handle Alpha here as weel
                stateSet->addState(BlendState::getOrCreate(blendState->srcRGB(), blendState->dstRGB()));
            }
            else if (qobject_cast<QCullFace *>(renderState) != Q_NULLPTR) {
                QCullFace *cullFace = qobject_cast<QCullFace *>(renderState);
                stateSet->addState(CullFace::getOrCreate(cullFace->mode()));
            }
            else if (qobject_cast<QDepthMask *>(renderState) != Q_NULLPTR) {
                QDepthMask *depthMask = qobject_cast<QDepthMask *>(renderState);
                stateSet->addState(DepthMask::getOrCreate(depthMask->mask()));
            }
            else if (qobject_cast<QDepthTest *>(renderState) != Q_NULLPTR) {
                QDepthTest *depthTest = qobject_cast<QDepthTest *>(renderState);
                stateSet->addState(DepthTest::getOrCreate(depthTest->func()));
            }
            else if (qobject_cast<QDithering *>(renderState) != Q_NULLPTR) {
                stateSet->addState(Dithering::getOrCreate());
            }
            else if (qobject_cast<QFrontFace *>(renderState) != Q_NULLPTR) {
                QFrontFace *frontFace = qobject_cast<QFrontFace *>(renderState);
                stateSet->addState(FrontFace::getOrCreate(frontFace->direction()));
            }
            else if (qobject_cast<QScissorTest *>(renderState) != Q_NULLPTR) {
                QScissorTest *scissorTest = qobject_cast<QScissorTest *>(renderState);
                stateSet->addState(ScissorTest::getOrCreate(scissorTest->left(),
                                                            scissorTest->bottom(),
                                                            scissorTest->width(),
                                                            scissorTest->height()));
            }
            else if (qobject_cast<QStencilTest *>(renderState) != Q_NULLPTR) {
                QStencilTest *stencilTest = qobject_cast<QStencilTest*>(renderState);
                stateSet->addState(StencilTest::getOrCreate(stencilTest->mask(),
                                                            stencilTest->func(),
                                                            stencilTest->faceMode()));
            }
        }

        return stateSet;
    }
    return Q_NULLPTR;
}

void RenderView::setUniformValue(QUniformPack &uniformPack, const QString &name, const QVariant &value)
{
    QTexture *tex = Q_NULLPTR;
    if (static_cast<QMetaType::Type>(value.type()) == QMetaType::QObjectStar &&
            (tex = value.value<Qt3D::QTexture *>()) != Q_NULLPTR) {
        uniformPack.setTexture(name, tex->uuid());
        TextureUniform *texUniform = m_allocator->allocate<TextureUniform>();
        texUniform->setTextureId(tex->uuid());
        uniformPack.setUniform(name, texUniform);
    }
    else {
        uniformPack.setUniform(name, QUniformValue::fromVariant(value, m_allocator));
    }
}

void RenderView::setShaderAndUniforms(RenderCommand *command, RenderRenderPass *rPass, QHash<QString, QVariant> &parameters, const QMatrix4x4 &worldTransform)
{
    // The VAO Handle is set directly in the renderer thread so as to avoid having to use a mutex here
    // Set shader, technique, and effect by basically doing :
    // ShaderProgramManager[MaterialManager[frontentEntity->uuid()]->Effect->Techniques[TechniqueFilter->name]->RenderPasses[RenderPassFilter->name]];
    // The Renderer knows that if one of those is null, a default material / technique / effect as to be used

    // Find all RenderPasses (in order) matching values set in the RenderPassFilter
    // Get list of parameters for the Material, Effect, and Technique
    // For each ParameterBinder in the RenderPass -> create a QUniformPack
    // Once that works, improve that to try and minimize QUniformPack updates

    if (rPass != Q_NULLPTR)
    {
        // Index RenderShader by Shader UUID
        command->m_shader = m_renderer->shaderManager()->lookupHandle(rPass->shaderProgram());
        RenderShader *shader = Q_NULLPTR;
        if ((shader = m_renderer->shaderManager()->data(command->m_shader)) != Q_NULLPTR) {

            // Builds the QUniformPack, sets shader standard uniforms and store attributes name / glname bindings
            // If a parameter is defined and not found in the bindings it is assumed to be a binding of Uniform type with the glsl name
            // equals to the parameter name
            QStringList uniformNames = shader->uniformsNames();
            QStringList attributeNames = shader->attributesNames();

            // Set fragData Name and index
            // Later on we might want to relink the shader if attachments have changed
            // But for now we set them once and for all
            QHash<QString, int> fragOutputs;
            if (!m_renderTarget.isNull() && !shader->isLoaded()) {
                Q_FOREACH (const Attachment &att, m_attachmentPack.attachments()) {
                    if (att.m_type <= QRenderAttachment::ColorAttachment15)
                        fragOutputs.insert(att.m_name, att.m_type);
                }
            }

            if (!uniformNames.isEmpty() || !attributeNames.isEmpty()) {

                // Set default standard uniforms without bindings
                Q_FOREACH (const QString &uniformName, uniformNames) {
                    if (m_standardUniformSetters.contains(uniformName))
                        command->m_uniforms.setUniform(uniformName,
                                                       (this->*m_standardUniformSetters[uniformName])(worldTransform));
                }

                // Set default attributes
                Q_FOREACH (const QString &attributeName, attributeNames) {
                    if (m_standardAttributesNames.contains(attributeName))
                        command->m_parameterAttributeToShaderNames[attributeName] = attributeName;
                }

                // Set uniforms and attributes explicitly binded
                Q_FOREACH (QParameterMapper *binding, rPass->bindings()) {
                    if (!parameters.contains(binding->parameterName())) {
                        if (binding->bindingType() == QParameterMapper::Attribute
                                && attributeNames.contains(binding->shaderVariableName()))
                            command->m_parameterAttributeToShaderNames.insert(binding->parameterName(), binding->shaderVariableName());
                        else if (binding->bindingType() == QParameterMapper::StandardUniform
                                 && uniformNames.contains(binding->shaderVariableName())
                                 && m_standardUniformSetters.contains(binding->parameterName()))
                            command->m_uniforms.setUniform(binding->shaderVariableName(),
                                                           (this->*m_standardUniformSetters[binding->parameterName()])(worldTransform));
                        else if (binding->bindingType() == QParameterMapper::FragmentOutput && fragOutputs.contains(binding->parameterName()))
                            fragOutputs.insert(binding->shaderVariableName(), fragOutputs.take(binding->parameterName()));
                        else
                            qCWarning(Render::Backend) << Q_FUNC_INFO << "Trying to bind a Parameter that hasn't been defined " << binding->parameterName();
                    }
                    else {
                        setUniformValue(command->m_uniforms, binding->shaderVariableName(), parameters.take(binding->parameterName()));
                    }
                }

                // If there are remaining parameters, those are set as uniforms
                Q_FOREACH (const QString &paramName, parameters.keys()) {
                    if (uniformNames.contains(paramName))
                        setUniformValue(command->m_uniforms, paramName, parameters.take(paramName));
                    // Else param unused by current shader
                }

                // Sets lights in shader
                // Maybe we should provide a flag in the shader to specify if it needs lights
                // that might save a bit of performances
                QHash<QString, int> m_countOfLightTypes;
                Q_FOREACH (const LightPair &lightPair, m_lights) {
                    RenderLight *light = m_renderer->lightManager()->data(lightPair.first);
                    if (light != Q_NULLPTR) {
                        if (!m_countOfLightTypes.contains(light->lightBlockName()))
                            m_countOfLightTypes[light->lightBlockName()] = 0;
                        int lightIdx = m_countOfLightTypes[light->lightBlockName()]++;

                        QHash<QString, QVariant> lightProperties = light->lightProperties();
                        lightProperties[QStringLiteral("position")] = lightPair.second.map(QVector3D());
                        Q_FOREACH (const QString &propertyName, lightProperties.keys()) {
                            QString lightShaderUniform(light->lightUniformName() + QStringLiteral("[") + QString::number(lightIdx) + QStringLiteral("].") + propertyName);
                            if (uniformNames.contains(lightShaderUniform)) {
                                command->m_uniforms.setUniform(lightShaderUniform, QUniformValue::fromVariant(lightProperties[propertyName], m_allocator));
                            }
                        }
                    }
                }
            }
            // Set frag outputs in the shaders if hash not empty
            if (!fragOutputs.empty())
                shader->setFragOutputs(fragOutputs);
        }
    }
    else {
        qCWarning(Render::Backend) << Q_FUNC_INFO << "Using default effect as none was provided";
    }
}

void RenderView::computeViewport(ViewportNode *viewportNode)
{
    QRectF tmpViewport(viewportNode->xMin(),
                       viewportNode->yMin(),
                       viewportNode->xMax(),
                       viewportNode->yMax());
    if (m_viewport == Q_NULLPTR) {
        m_viewport = m_allocator->allocate<QRectF>();
        *m_viewport = tmpViewport;
    }
    else {
        QRectF oldViewport = *m_viewport;
        *m_viewport = QRectF(tmpViewport.x() + tmpViewport.width() * oldViewport.x(),
                             tmpViewport.y() + tmpViewport.height() * oldViewport.y(),
                             tmpViewport.width() * oldViewport.width(),
                             tmpViewport.height() * oldViewport.height());
    }
    // So that we use the color of the highest viewport
    m_clearColor = m_allocator->allocate<QColor>();
    *m_clearColor = viewportNode->clearColor();
}

bool RenderView::checkContainedWithinLayer(RenderEntity *node)
{
    if (m_layers.isEmpty())
        return true;
    RenderLayer *renderLayer = node->renderComponent<RenderLayer>();
    if (renderLayer == Q_NULLPTR || !m_layers.contains(renderLayer->layer()))
        return false;
    return true;
}

} // namespace Render
} // namespace Qt3D

QT_END_NAMESPACE
