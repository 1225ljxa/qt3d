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

#include "renderview.h"
#include "material.h"
#include "renderer.h"
#include "rendercamera.h"
#include "rendercommand.h"
#include "rendernode.h"
#include "meshdatamanager.h"
#include "vaomanager.h"
#include "meshmanager.h"
#include "meshdata.h"
#include "cameramanager.h"
#include "rendernodesmanager.h"
#include "materialmanager.h"
#include "techniquemanager.h"
#include "shadermanager.h"
#include <cameraselectornode.h>
#include <framegraphnode.h>
#include <renderpassfilternode.h>
#include <techniquefilternode.h>
#include <viewportnode.h>
#include "rendereffect.h"
#include "effectmanager.h"
#include "renderlogging.h"
#include "renderpassmanager.h"
#include "renderrenderpass.h"
#include "parameterbinder.h"
#include "parameter.h"

#include <Qt3DCore/entity.h>
#include <Qt3DCore/qabstracteffect.h>
#include <Qt3DCore/qabstracttechnique.h>

#include <QDebug>

QT_BEGIN_NAMESPACE

namespace Qt3D {
namespace Render {

RenderView::RenderView()
    : m_renderer(Q_NULLPTR)
    , m_techniqueFilter(0)
    , m_passFilter(0)
    , m_commands()
{
}

RenderView::~RenderView()
{
    qDeleteAll(m_commands);
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
        case FrameGraphNode::CameraSelector: {
            CameraSelector *cameraSelector = static_cast<CameraSelector *>(node);
            Entity *cameraEntity = cameraSelector->cameraEntity();
            if (cameraEntity != Q_NULLPTR) {
                m_camera = m_renderer->cameraManager()->lookupHandle(cameraEntity->uuid());
                RenderCamera *tmpCam = m_renderer->cameraManager()->data(m_camera);
                RenderNode *tmpCamNode = m_renderer->renderNodesManager()->lookupResource(cameraEntity->uuid());
                if (tmpCam && tmpCamNode)
                    tmpCam->setViewMatrix(*tmpCamNode->worldTransform());
            }
            break;
        }

        case FrameGraphNode::LayerFilter:
            qCWarning(Backend) << "LayerFilter not implemented yet";
            break;

        case FrameGraphNode::RenderPassFilter:
            m_passFilter = static_cast<RenderPassFilter *>(node);
            break;

        case FrameGraphNode::RenderTarget:
            qCWarning(Backend) << "RenderTarget not implemented yet";
            break;

        case FrameGraphNode::TechniqueFilter:
            m_techniqueFilter = static_cast<TechniqueFilter *>(node);
            break;

        case FrameGraphNode::Viewport:
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

    // The goal here is to sort RenderCommand by :
    // 1) Shader
    // 2) DrawStateSet
    // 2) Texture
}

void RenderView::setRenderer(Renderer *renderer)
{
    m_renderer = renderer;
}

// Traverse Scene graphTree or culledSceneGraphTree
// ideally m_commands has been sized properly after the
// scene has been culled to the number of nodes in the culled
// scene using reserve().
// Tries to order renderCommand by shader so as to minimize shader changes
void RenderView::buildRenderCommands(RenderNode *node)
{
    // Build renderCommand for current node

    // 1 RenderCommand per RenderPass pass on an Entity with a Mesh

    Entity *frontEndEntity = Q_NULLPTR;
    if (node->frontEndPeer() != Q_NULLPTR
            && (frontEndEntity = node->frontEndPeer()->asEntity()) != Q_NULLPTR) {
        HMesh meshHandle;
        if (m_renderer->meshManager()->contains(frontEndEntity->uuid()) &&
                (meshHandle = m_renderer->meshManager()->lookupHandle(frontEndEntity->uuid())) != HMesh()) {
            RenderMesh *mesh = m_renderer->meshManager()->data(meshHandle);
            if (mesh == Q_NULLPTR || mesh->peer() == Q_NULLPTR)
                return ;
            if (mesh->meshDirty()) {
                mesh->setMeshData(m_renderer->meshDataManager()->lookupHandle(mesh->peer()->uuid()));
                qCDebug(Backend) << Q_FUNC_INFO << "Updating RenderMesh -> MeshData handle";
            }

            RenderMaterial *material = findMaterialForMeshNode(frontEndEntity->uuid());
            RenderEffect *effect = findEffectForMaterial(material);
            RenderTechnique *technique = findTechniqueForEffect(effect);
            QList<RenderRenderPass *> passes = findRenderPassesForTechnique(technique);
            QHash<QString, Parameter *> parameters = parametersFromMaterialEffectTechnique(material, effect, technique);

            Q_FOREACH (RenderRenderPass *pass, passes) {
                RenderCommand *command = new RenderCommand();
                command->m_mesh = meshHandle;
                command->m_meshData = mesh->meshData();
                command->m_instancesCount = 0;
                command->m_worldMatrix = *(node->worldTransform());
                command->m_stateSet = Q_NULLPTR;
                setShaderAndUniforms(command, pass, parameters);
                m_commands.append(command);
            }

            if (passes.isEmpty()) {
                qCritical() << "No RenderPasses found. Make sure you have properly set your Material, Technique and Effect. Rendering using default RenderPass";
                RenderCommand *command = new RenderCommand();
                command->m_mesh = meshHandle;
                command->m_meshData = mesh->meshData();
                command->m_instancesCount = 0;
                command->m_worldMatrix = *(node->worldTransform());
                command->m_stateSet = Q_NULLPTR;
                m_commands.append(command);
            }

        }
    }

    // Traverse children
    foreach (RenderNode *child, node->children()) {
        buildRenderCommands(child);
    }
}

QRectF RenderView::viewport() const
{
    return m_viewport;
}

RenderCamera *RenderView::camera() const
{
    return m_renderer->cameraManager()->data(m_camera);
}

RenderMaterial *RenderView::findMaterialForMeshNode(const QUuid &entityUuid)
{
    // Material is created by the RenderSceneBuilder or RenderNode if it is appended through a change
    // Therefore we only perform lookups in RenderView
    return m_renderer->materialManager()->renderMaterial(entityUuid);
}

// The RenderTechnique && RenderPass instances have to be created in the RenderViewJobs
// As it offers a way to create instances only for techniques and passes that are used.

RenderEffect *RenderView::findEffectForMaterial(RenderMaterial *material)
{
    RenderEffect *effect = Q_NULLPTR;
    if (material != Q_NULLPTR && material->peer() != Q_NULLPTR) {
        QAbstractEffect *fEffect = material->peer()->effect();
        // TO DO : Replace the locker by locking policies in the Resources Managers
        if (fEffect != Q_NULLPTR && (effect = m_renderer->effectManager()->lookupResource(fEffect)) == Q_NULLPTR) {
            effect = m_renderer->effectManager()->getOrCreateResource(fEffect);
            effect->setRendererAspect(m_renderer->rendererAspect());
            effect->setPeer(fEffect);
        }
    }
    return effect;
}

RenderTechnique *RenderView::findTechniqueForEffect(RenderEffect *effect)
{
    // Check to see if the Effect contains a Technique matching the criteria defined in the Technique Filter
    // Effect (RenderEffect) contains a list of QAbstractTechnique that is updated to reflect the one of its frontend Effect
    // using the QChangeArbiter to avoid race conditions
    // On the other hand, we should maybe rework the filtering below as it accesses the criteria of the Frontend QML texture
    // meaning that those could be changed while we're reading them.
    // That would imply creating a RenderTechnique for each Technique defined in the Effect and having each RenderTechnique contain
    // a list of criteria
    // If TechniqueCriteria and their values were defined as constant, the filtering below would be enough

    // Can we fully perform the technique selection here ?
    // If we need to check for extension / vendor / OpenGL Version we would need access to
    // The QGraphicContext which is only avalaible in the Renderer's thread
    // Current Plan : Have an interface accessible through the Renderer from that thread where we have
    // all the needed information : Vendor, Extensions, GL Versions .... so that we can perform the checks here
    // Also we could define that for defined criterionTypes (Vendor, GL_Version ...) the technique filtering doesn't
    // depend on the TechniqueFilter but entirely on what the system GL interface gives us
    // Furthermode, finding a way to perform this filtering as little as possible could provide some performance improvements
    if (effect != Q_NULLPTR) {
        Technique *tech = Q_NULLPTR;
        Q_FOREACH (QAbstractTechnique *technique, effect->techniques()) {
            if ((tech = qobject_cast<Technique*>(technique)) != Q_NULLPTR) {
                bool foundMatchingTechnique = true;
                Q_FOREACH (TechniqueCriterion *filterCriterion, m_techniqueFilter->filters()) {
                    bool findMatch = false;
                    Q_FOREACH (TechniqueCriterion *techCriterion, tech->criteria()) {
                        if (*filterCriterion == *techCriterion) {
                            findMatch = true;
                            break;
                        }
                    }
                    if (!findMatch) {
                        foundMatchingTechnique = false;
                        break;
                    }
                }
                if (!foundMatchingTechnique)
                    qFatal("No Technique was found matching the criteria defined in the TechniqueFilter : Cannot Continue");
            }
        }
        RenderTechnique *technique = Q_NULLPTR;
        if ((technique = m_renderer->techniqueManager()->lookupResource(tech)) == Q_NULLPTR) {
            // Tries to select technique based on criteria defined in the TechniqueFilter
            Q_FOREACH (QAbstractTechnique *t, effect->techniques()) {
                if (qobject_cast<Technique *>(t) && t == tech) {
                    technique = m_renderer->techniqueManager()->getOrCreateResource(t);
                    technique->setRenderer(m_renderer);
                    technique->setPeer(qobject_cast<Technique *>(t));
                    break;
                }
            }
        }
        if (technique == Q_NULLPTR)
            qCCritical(Render::Backend) << Q_FUNC_INFO << "No technique found for technique filter";
        return technique;
    }
    return Q_NULLPTR;
}

QList<RenderRenderPass *> RenderView::findRenderPassesForTechnique(RenderTechnique *technique)
{
    QList<RenderRenderPass *> passes;
    if (technique != Q_NULLPTR && technique->peer() != Q_NULLPTR) {
        // TO DO : Improve so we handle the case where the peer or renderPasses in the peer change
        Q_FOREACH (QAbstractRenderPass *pass, technique->peer()->renderPasses()) {
            // TO DO : IF PASS MATCHES CRITERIA
            RenderRenderPass *rPass = m_renderer->renderPassManager()->lookupResource(pass);
            if (rPass == Q_NULLPTR) {
                rPass = m_renderer->renderPassManager()->getOrCreateResource(pass);
                rPass->setRenderer(m_renderer);
                rPass->setPeer(qobject_cast<RenderPass*>(pass));
            }
            passes << rPass;
        }
    }
    return passes;
}

QHash<QString, Parameter *> RenderView::parametersFromMaterialEffectTechnique(RenderMaterial *material,
                                                                              RenderEffect *effect,
                                                                              RenderTechnique *technique)
{
    QHash<QString, Parameter *> params;

    // TO DO : To complete by addind parameters to effect and technique
    // Material is preferred over Effect
    // Effect is preferred over Technique
    // By filling the hash in reverse preference order, we're ensured that we preserve preference

    //    Q_FOREACH (Parameter *param, technique->parameters()) {
    //
    //    }

    //    Q_FOREACH (Parameter *param, effect->parameters()) {
    //
    //    }

    if (material != Q_NULLPTR)
        Q_FOREACH (Parameter *param, material->parameters()) {
            params[param->name()] = param;
        }

    return params;
}

void RenderView::setShaderAndUniforms(RenderCommand *command, RenderRenderPass *rPass, const QHash<QString, Parameter *> parameters)
{
    // The VAO Handle is set directly in the renderer thread so as to avoid having to use a mutex here
    // Set shader, technique, and effect by basically doing :
    // ShaderProgramManager[MaterialManager[frontentEntity->uuid()]->Effect->Techniques[TechniqueFilter->name]->RenderPasses[RenderPassFilter->name]];
    // The Renderer knows that if one of those is null, a default material / technique / effect as to be used

    // Find all RenderPasses (in order) matching values set in the RenderPassFilter
    // Get list of parameters for the Material, Effect, and Technique
    // For each ParameterBinder in the RenderPass -> create a QUniformPack
    // Once that works, improve that to try and minimize QUniformPack updates
    // Get Parameters only from Material as a first step

    if (rPass != Q_NULLPTR && rPass->shaderProgram() != Q_NULLPTR) {
        // Index RenderShader by Shader UUID
        command->m_shader = m_renderer->shaderManager()->lookupHandle(rPass->shaderProgram()->uuid());
        RenderShader *shader = Q_NULLPTR;
        if ((shader = m_renderer->shaderManager()->data(command->m_shader)) == Q_NULLPTR) {
            shader = m_renderer->shaderManager()->getOrCreateResource(rPass->shaderProgram()->uuid());
            shader->setPeer(qobject_cast<ShaderProgram*>(rPass->shaderProgram()));
            command->m_shader = m_renderer->shaderManager()->lookupHandle(rPass->shaderProgram()->uuid());
        }
        // TO DO : To be corrected later on
        //        command->m_stateSet = qobject_cast<RenderPass*>(pass)->stateSet();

        // Builds the QUniformPack, sets shader standard uniforms and store attributes name / glname bindings
        Q_FOREACH (ParameterBinder *binding, rPass->bindings()) {
            if (binding->bindingType() == ParameterBinder::Uniform) {
                if (!parameters.contains(binding->parameterName())) {
                    qCCritical(Render::Backend) << Q_FUNC_INFO << "Trying to bind a Parameter that hasn't been defined " << binding->parameterName();
                }
                else {
                    Parameter *param = parameters[binding->parameterName()];
                    if (!param->isStandardUniform()) {
                        if (param->datatype() >= Parameter::Float && param->datatype() <= Parameter::FloatMat4)
                            command->m_uniforms.setUniform(binding->shaderVariableName(), QUniformValue(QUniformValue::Float, param->value()));
                        else if (param->datatype() >= Parameter::Int)
                            command->m_uniforms.setUniform(binding->shaderVariableName(), QUniformValue(QUniformValue::Int, param->value()));
                    }
                    else {
                        shader->setStandardUniform(param->standardUniform(), binding->shaderVariableName());
                    }
                }
            }
            else { // Attribute
                command->m_parameterAttributeToShaderNames[binding->parameterName()] = binding->shaderVariableName();
            }
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
    if (m_viewport.isEmpty()) {
        m_viewport = tmpViewport;
    }
    else {
        QRectF oldViewport = m_viewport;
        m_viewport = QRectF(tmpViewport.x() + tmpViewport.width() * oldViewport.x(),
                            tmpViewport.y() + tmpViewport.height() * oldViewport.y(),
                            tmpViewport.width() * oldViewport.width(),
                            tmpViewport.height() * oldViewport.height());
    }
}

} // namespace Render
} // namespace Qt3D

QT_END_NAMESPACE
