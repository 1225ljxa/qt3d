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

#include "renderviewjobutils_p.h"

#include <Qt3DRenderer/qopenglfilter.h>
#include <Qt3DRenderer/sphere.h>

#include <Qt3DRenderer/private/cameraselectornode_p.h>
#include <Qt3DRenderer/private/clearbuffer_p.h>
#include <Qt3DRenderer/private/layerfilternode_p.h>
#include <Qt3DRenderer/private/managers_p.h>
#include <Qt3DRenderer/private/rendereffect_p.h>
#include <Qt3DRenderer/private/renderpassfilternode_p.h>
#include <Qt3DRenderer/private/rendertargetselectornode_p.h>
#include <Qt3DRenderer/private/renderview_p.h>
#include <Qt3DRenderer/private/sortmethod_p.h>
#include <Qt3DRenderer/private/techniquefilternode_p.h>
#include <Qt3DRenderer/private/viewportnode_p.h>


QT_BEGIN_NAMESPACE

namespace Qt3D {
namespace Render {

/*!
    \internal
    Walks up the framegraph tree from \p fgLeaf and builds up as much state
    as possible and populates \p rv. For cases where we can't get the specific state
    (e.g. because it depends upon more than just the framegraph) we store the data from
    the framegraph that will be needed to later when the rest of the data becomes available
*/
void setRenderViewConfigFromFrameGraphLeafNode(RenderView *rv, const FrameGraphNode *fgLeaf)
{
    // The specific RenderPass to be used is also dependent upon the Effect and TechniqueFilter
    // which is referenced by the Material which is referenced by the RenderMesh. So we can
    // only store the filter info in the RenderView structure and use it to do the resolving
    // when we build the RenderCommand list.
    const Renderer *renderer = rv->renderer();
    const FrameGraphNode *node = fgLeaf;

    while (node) {
        FrameGraphNode::FrameGraphNodeType type = node->nodeType();
        switch (type) {
        case FrameGraphNode::CameraSelector:
            // Can be set only once and we take camera nearest to the leaf node
            if (!rv->renderCamera()) {
                const CameraSelector *cameraSelector = static_cast<const CameraSelector *>(node);
                RenderEntity *camNode = renderer->renderNodesManager()->lookupResource(cameraSelector->cameraUuid());
                if (camNode) {
                    rv->setRenderCamera(camNode->renderComponent<RenderCameraLens>());
                    rv->setViewMatrix(*camNode->worldTransform());

                    // TODO: We can extract camera pos from the modelview matrix
                    rv->setEyePosition(camNode->worldBoundingVolume()->center());
                }
                break;
            }

        case FrameGraphNode::LayerFilter: // Can be set multiple times in the tree
            rv->appendLayerFilter(static_cast<const LayerFilterNode *>(node)->layers());
            break;

        case FrameGraphNode::RenderPassFilter:
            // Can be set once
            // TODO: Amalgamate all render pass filters from leaf to root
            if (!rv->renderPassFilter())
                rv->setRenderPassFilter(static_cast<const RenderPassFilter *>(node));
            break;

        case FrameGraphNode::RenderTarget: {
            // Can be set once and we take render target nearest to the leaf node
            QNodeUuid renderTargetUid = static_cast<const RenderTargetSelector *>(node)->renderTargetUuid();
            HTarget renderTargetHandle = renderer->renderTargetManager()->lookupHandle(renderTargetUid);
            if (rv->renderTargetHandle().isNull()) {
                rv->setRenderTargetHandle(renderTargetHandle);

                RenderTarget *renderTarget = renderer->renderTargetManager()->data(renderTargetHandle);
                if (renderTarget) {
                    // Add renderTarget Handle and build renderCommand AttachmentPack
                    Q_FOREACH (const QNodeUuid &attachmentId, renderTarget->renderAttachments()) {
                        RenderAttachment *attachment = renderer->attachmentManager()->lookupResource(attachmentId);
                        if (attachment)
                            rv->addRenderAttachment(attachment->attachment());
                    }
                }
            }
            break;
        }

        case FrameGraphNode::ClearBuffer:
            rv->setClearBuffer(static_cast<const ClearBuffer *>(node)->type());
            break;

        case FrameGraphNode::TechniqueFilter:
            // Can be set once
            // TODO Amalgamate all technique filters from leaf to root
            if (rv->techniqueFilter())
                rv->setTechniqueFilter(static_cast<const TechniqueFilter *>(node));
            break;

        case FrameGraphNode::Viewport: {
            // If the Viewport has already been set in a lower node
            // Make it so that the new viewport is actually
            // a subregion relative to that of the parent viewport
            const ViewportNode *vpNode = static_cast<const ViewportNode *>(node);
            rv->setViewport(computeViewport(rv->viewport(), vpNode));

            // We take the clear color from the viewport node nearest the leaf
            if (!rv->clearColor().isValid())
                rv->setClearColor(vpNode->clearColor());
            break;
        }

        case FrameGraphNode::SortMethod: {
            const Render::SortMethod *sortMethod = static_cast<const Render::SortMethod *>(node);
            rv->addSortCriteria(sortMethod->criteria());
            break;
        }

        default:
            // Should never get here
            qCWarning(Backend) << "Unhandled FrameGraphNode type";
        }

        node = node->parent();
    }
}

/*!
    \internal
    Walks the scene graph of RenderEntities rooted at \p node and collects
    together any resources needed by the \p rv.
*/
void preprocessRenderTree(RenderView *rv, const RenderEntity *node)
{
    // TODO: Reimplement this without any reference to lights. At the lowest common
    // level this boils down to assembling uniform arrays or uniform buffers to pass
    // into the shader programs. Qt3D may provide some default light types but they
    // should use the same generic methods as any user data that needs to get into
    // uniforms.

    // Retrieve light for the currentNode and append it to list of current lights
    // As only light components of an Entity are considered active
    // Note : Layer filtering isn't applied there
    //
    // TODO: Perhaps make this block of code configurable by allowing the Technique
    // or similar to provide a functor?
    HLight lightHandle = node->componentHandle<RenderLight, 16>();
    if (!lightHandle.isNull())
        rv->appendLight(lightHandle, *node->worldTransform());

    // Traverse children
    Q_FOREACH (RenderEntity *child, node->children())
        preprocessRenderTree(rv, child);
}

/*!
    \internal
    Searches the \a renderer for the best matching RenderTechnique from
    \a effect specified by the \a renderView.
*/
RenderTechnique *findTechniqueForEffect(Renderer *renderer,
                                        RenderView *renderView,
                                        RenderEffect *effect)
{
    if (!effect)
        return Q_NULLPTR;

    // Iterate through the techniques in the effect
    Q_FOREACH (const QNodeUuid &techniqueId, effect->techniques()) {
        RenderTechnique *technique = renderer->techniqueManager()->lookupResource(techniqueId);

        if (!technique)
            continue;

        if (*renderer->contextInfo() == *technique->openGLFilter()) {

            // If no techniqueFilter is present, we return the technique as it satisfies OpenGL version
            const TechniqueFilter *techniqueFilter = renderView->techniqueFilter();
            bool foundMatch = (techniqueFilter == Q_NULLPTR || techniqueFilter->filters().isEmpty());
            if (foundMatch)
                return technique;

            // There is a technique filter so we need to check for a technique with suitable criteria.
            // Check for early bail out if the technique doesn't have sufficient number of criteria and
            // can therefore never satisfy the filter
            if (technique->annotations().size() < techniqueFilter->filters().size())
                continue;

            // Iterate through the filter criteria and for each one search for a criteria on the
            // technique that satisfies it
            Q_FOREACH (const QUuid &filterAnnotationId, techniqueFilter->filters()) {
                foundMatch = false;
                RenderAnnotation *filterAnnotation = renderer->criterionManager()->lookupResource(filterAnnotationId);

                Q_FOREACH (const QUuid &techniqueAnnotationId, technique->annotations()) {
                    RenderAnnotation *techniqueAnnotation = renderer->criterionManager()->lookupResource(techniqueAnnotationId);
                    if ((foundMatch = (*techniqueAnnotation == *filterAnnotation)))
                        break;
                }

                if (!foundMatch) {
                    // No match for TechniqueFilter criterion in any of the technique's criteria.
                    // So no way this can match. Don't bother checking the rest of the criteria.
                    break;
                }
            }

            if (foundMatch)
                return technique; // All criteria matched - we have a winner!
        }
    }

    // We failed to find a suitable technique to use :(
    return Q_NULLPTR;
}


QVector<RenderRenderPass *> findRenderPassesForTechnique(Renderer *renderer,
                                                         RenderView *renderView,
                                                         RenderTechnique *technique)
{
    Q_ASSERT(renderer);
    Q_ASSERT(technique);

    QVector<RenderRenderPass *> passes;
    passes.reserve(3); // We rarely get more than 2 or 3 passes
    Q_FOREACH (const QUuid &passId, technique->renderPasses()) {
        RenderRenderPass *renderPass = renderer->renderPassManager()->lookupResource(passId);

        if (renderPass) {
            const RenderPassFilter *passFilter = renderView->renderPassFilter();
            bool foundMatch = (!passFilter || passFilter->filters().size() == 0);

            // A pass filter is present so we need to check for matching criteria
            if (!foundMatch && renderPass->annotations().size() >= passFilter->filters().size()) {

                // Iterate through the filter criteria and look for render passes with criteria that satisfy them
                Q_FOREACH (const QUuid &filterAnnotationId, passFilter->filters()) {
                    foundMatch = false;
                    RenderAnnotation *filterAnnotation = renderer->criterionManager()->lookupResource(filterAnnotationId);

                    Q_FOREACH (const QUuid &passAnnotationId, renderPass->annotations()) {
                        RenderAnnotation *passAnnotation = renderer->criterionManager()->lookupResource(passAnnotationId);
                        if ((foundMatch = (*passAnnotation == *filterAnnotation)))
                            break;
                    }

                    if (!foundMatch) {
                        // No match for criterion in any of the render pass' criteria
                        break;
                    }
                }
            }

            if (foundMatch) {
                // Found a renderpass that satisfies our needs. Add it in order
                passes << renderPass;
            }
        }
    }

    return passes;
}

} // namespace Render
} // namespace Qt3D

QT_END_NAMESPACE
