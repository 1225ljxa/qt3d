/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <Qt3DRender/private/rendertargetoutput_p.h>
#include <Qt3DCore/qscenepropertychange.h>
#include <Qt3DRender/qtexture.h>
#include <QVariant>

QT_BEGIN_NAMESPACE

using namespace Qt3DCore;

namespace Qt3DRender {
namespace Render {

RenderTargetOutput::RenderTargetOutput()
    : BackendNode()
{
}

void RenderTargetOutput::updateFromPeer(Qt3DCore::QNode *peer)
{
    QRenderTargetOutput *attachment = static_cast<QRenderTargetOutput *>(peer);

    m_attachmentData.m_mipLevel = attachment->mipLevel();
    m_attachmentData.m_layer = attachment->layer();
    m_attachmentData.m_point = attachment->attachmentPoint();
    m_attachmentData.m_face = attachment->face();
    if (attachment->texture())
        m_attachmentData.m_textureUuid = attachment->texture()->id();
}

Qt3DCore::QNodeId RenderTargetOutput::textureUuid() const
{
    return m_attachmentData.m_textureUuid;
}

int RenderTargetOutput::mipLevel() const
{
    return m_attachmentData.m_mipLevel;
}

int RenderTargetOutput::layer() const
{
    return m_attachmentData.m_layer;
}

QString RenderTargetOutput::name() const
{
    return m_attachmentData.m_name;
}

QAbstractTexture::CubeMapFace RenderTargetOutput::face() const
{
    return m_attachmentData.m_face;
}

QRenderTargetOutput::AttachmentPoint RenderTargetOutput::point() const
{
    return m_attachmentData.m_point;
}

void RenderTargetOutput::sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e)
{
    QScenePropertyChangePtr propertyChange = qSharedPointerCast<QScenePropertyChange>(e);
    if (e->type() == NodeUpdated) {
        if (propertyChange->propertyName() == QByteArrayLiteral("type")) {
            m_attachmentData.m_point = static_cast<QRenderTargetOutput::AttachmentPoint>(propertyChange->value().toInt());
        }
        else if (propertyChange->propertyName() == QByteArrayLiteral("texture")) {
            m_attachmentData.m_textureUuid = propertyChange->value().value<QNodeId>();
        }
        else if (propertyChange->propertyName() == QByteArrayLiteral("mipLevel")) {
            m_attachmentData.m_mipLevel = propertyChange->value().toInt();
        }
        else if (propertyChange->propertyName() == QByteArrayLiteral("layer")) {
            m_attachmentData.m_layer = propertyChange->value().toInt();
        }
        else if (propertyChange->propertyName() == QByteArrayLiteral("face")) {
            m_attachmentData.m_face = static_cast<QAbstractTexture::CubeMapFace>(propertyChange->value().toInt());
        }
        markDirty(AbstractRenderer::AllDirty);
    }

    BackendNode::sceneChangeEvent(e);
}

Attachment RenderTargetOutput::attachment() const
{
    return m_attachmentData;
}

} // namespace Render
} // namespace Qt3DRender

QT_END_NAMESPACE
