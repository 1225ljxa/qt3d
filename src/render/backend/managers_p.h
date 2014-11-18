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

#ifndef QT3D_RENDER_MANAGERS_P_H
#define QT3D_RENDER_MANAGERS_P_H

#include <Qt3DCore/private/qresourcemanager_p.h>
#include <Qt3DRenderer/private/renderattachment_p.h>
#include <Qt3DRenderer/private/rendercameralens_p.h>
#include <Qt3DRenderer/private/renderannotation_p.h>
#include <Qt3DRenderer/private/rendereffect_p.h>
#include <Qt3DRenderer/private/renderentity_p.h>
#include <Qt3DRenderer/private/renderlayer_p.h>
#include <Qt3DRenderer/private/renderlight_p.h>
#include <Qt3DRenderer/private/rendermaterial_p.h>
#include <Qt3DRenderer/private/rendermesh_p.h>
#include <Qt3DRenderer/private/rendershader_p.h>
#include <Qt3DRenderer/private/sortcriterion_p.h>
#include <Qt3DRenderer/private/rendertechnique_p.h>
#include <Qt3DRenderer/private/rendertexture_p.h>
#include <Qt3DRenderer/private/rendertransform_p.h>
#include <Qt3DRenderer/private/rendertarget_p.h>
#include <Qt3DRenderer/private/renderrenderpass_p.h>
#include <Qt3DRenderer/private/renderparameter_p.h>
#include <Qt3DRenderer/private/handle_types_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

class QMeshData;

namespace Render {

class AttachmentManager : public QResourceManager<
        RenderAttachment,
        QNodeUuid,
        16,
        Qt3D::ArrayAllocatingPolicy,
        Qt3D::ObjectLevelLockingPolicy>
{
public:
    AttachmentManager() {}
};

class CameraManager : public QResourceManager<
        RenderCameraLens,
        QNodeUuid,
        8,
        Qt3D::ArrayAllocatingPolicy>
{
public:
    CameraManager() {}
};

class CriterionManager : public QResourceManager<
        RenderAnnotation,
        QNodeUuid,
        16,
        Qt3D::ArrayAllocatingPolicy,
        Qt3D::ObjectLevelLockingPolicy>

{
public:
    CriterionManager() {}
};

class EffectManager : public QResourceManager<
        RenderEffect,
        QNodeUuid,
        16,
        Qt3D::ArrayAllocatingPolicy,
        Qt3D::ObjectLevelLockingPolicy>
{
public:
    EffectManager() {}
};

class EntityManager : public QResourceManager<RenderEntity, QNodeUuid, 16>
{
public:
    EntityManager() {}
};

class FrameGraphManager : public QResourceManager<
        FrameGraphNode *,
        QNodeUuid,
        8,
        Qt3D::ArrayAllocatingPolicy>
{
public:
    FrameGraphManager() {}
};

class LayerManager : public QResourceManager<
        RenderLayer,
        QNodeUuid,
        16,
        Qt3D::ArrayAllocatingPolicy,
        Qt3D::ObjectLevelLockingPolicy>
{
public:
    LayerManager() {}
};

class LightManager : public QResourceManager<
        RenderLight,
        QNodeUuid,
        16,
        Qt3D::ArrayAllocatingPolicy,
        Qt3D::ObjectLevelLockingPolicy>
{
public:
    LightManager() {}
};

class MaterialManager : public QResourceManager<
        RenderMaterial,
        QNodeUuid,
        16,
        Qt3D::ArrayAllocatingPolicy,
        Qt3D::ObjectLevelLockingPolicy>
{
public:
    MaterialManager() {}
};

class MatrixManager : public QResourceManager<QMatrix4x4, QNodeUuid, 16>
{
public:
    MatrixManager() {}
};

class MeshManager : public QResourceManager<
        RenderMesh,
        QNodeUuid,
        16,
        Qt3D::ArrayAllocatingPolicy,
        Qt3D::ObjectLevelLockingPolicy>
{
public:
    MeshManager() {}
};

class ShaderManager : public QResourceManager<
        RenderShader,
        QNodeUuid,
        16,
        Qt3D::ArrayAllocatingPolicy,
        Qt3D::ObjectLevelLockingPolicy>
{
public:
    ShaderManager() {}
};

class SortCriterionManager : public QResourceManager<
        SortCriterion,
        QNodeUuid,
        8,
        Qt3D::ArrayAllocatingPolicy,
        Qt3D::ObjectLevelLockingPolicy>
{
public:
    SortCriterionManager() {}
};

class TechniqueManager : public QResourceManager<
        RenderTechnique,
        QNodeUuid,
        16,
        Qt3D::ArrayAllocatingPolicy,
        Qt3D::ObjectLevelLockingPolicy>
{
public:
    TechniqueManager() {}
};

class TextureManager : public QResourceManager<
        RenderTexture,
        QNodeUuid,
        16,
        Qt3D::ArrayAllocatingPolicy,
        Qt3D::ObjectLevelLockingPolicy>
{
public:
    TextureManager() {}
};

class TransformManager : public QResourceManager<
        RenderTransform,
        QNodeUuid,
        16,
        Qt3D::ArrayAllocatingPolicy,
        Qt3D::ObjectLevelLockingPolicy>
{
public:
    TransformManager() {}
};

class VAOManager : public QResourceManager<
        QOpenGLVertexArrayObject *,
        QPair<HMeshData, HShader>,
        16>
{
public:
    VAOManager() {}
};

class RenderTargetManager : public QResourceManager<
        RenderTarget,
        QNodeUuid,
        8,
        Qt3D::ArrayAllocatingPolicy,
        Qt3D::ObjectLevelLockingPolicy>
{
public:
    RenderTargetManager() {}
};

class RenderPassManager : public QResourceManager<
        RenderRenderPass,
        QNodeUuid,
        16,
        Qt3D::ArrayAllocatingPolicy,
        Qt3D::ObjectLevelLockingPolicy>
{
public:
    RenderPassManager() {}
};


class ParameterManager : public QResourceManager<
        RenderParameter,
        QNodeUuid,
        16,
        Qt3D::ArrayAllocatingPolicy,
        Qt3D::ObjectLevelLockingPolicy>
{
public:
    ParameterManager() {}
};

} // Render

Q_DECLARE_RESOURCE_INFO(Render::RenderAnnotation, Q_REQUIRES_CLEANUP);
Q_DECLARE_RESOURCE_INFO(Render::RenderEffect, Q_REQUIRES_CLEANUP);
Q_DECLARE_RESOURCE_INFO(Render::RenderEntity, Q_REQUIRES_CLEANUP);
Q_DECLARE_RESOURCE_INFO(Render::RenderLayer, Q_REQUIRES_CLEANUP);
Q_DECLARE_RESOURCE_INFO(Render::RenderLight, Q_REQUIRES_CLEANUP);
Q_DECLARE_RESOURCE_INFO(Render::RenderMaterial, Q_REQUIRES_CLEANUP);
Q_DECLARE_RESOURCE_INFO(Render::RenderMesh, Q_REQUIRES_CLEANUP);
Q_DECLARE_RESOURCE_INFO(Render::RenderShader, Q_REQUIRES_CLEANUP);
Q_DECLARE_RESOURCE_INFO(Render::SortCriterion, Q_REQUIRES_CLEANUP);
Q_DECLARE_RESOURCE_INFO(Render::RenderTarget, Q_REQUIRES_CLEANUP);
Q_DECLARE_RESOURCE_INFO(Render::RenderTechnique, Q_REQUIRES_CLEANUP);
Q_DECLARE_RESOURCE_INFO(Render::RenderTexture, Q_REQUIRES_CLEANUP);
Q_DECLARE_RESOURCE_INFO(Render::RenderRenderPass, Q_REQUIRES_CLEANUP);

} // Qt3D

QT_END_NAMESPACE


#endif // QT3D_RENDER_MANAGERS_P_H
