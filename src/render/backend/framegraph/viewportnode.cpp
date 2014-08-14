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

#include "viewportnode.h"

#include <Qt3DRenderer/renderer.h>
#include <Qt3DRenderer/rendereraspect.h>
#include <Qt3DRenderer/qviewport.h>

#include <Qt3DCore/qaspectmanager.h>
#include <Qt3DCore/qchangearbiter.h>
#include <Qt3DCore/qscenepropertychange.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {
namespace Render {

ViewportNode::ViewportNode()
    : FrameGraphNode(FrameGraphNode::Viewport)
    , m_xMin(0.0f)
    , m_yMin(0.0f)
    , m_xMax(1.0f)
    , m_yMax(1.0f)
{
}

void ViewportNode::setPeer(Qt3D::QViewport *peer)
{
    QUuid peerUuid;
    if (peer != Q_NULLPTR)
        peerUuid = peer->uuid();
    if (m_frontendUuid != peerUuid) {
        if (!m_frontendUuid.isNull()) {
            m_renderer->rendererAspect()->aspectManager()->changeArbiter()->unregisterObserver(this, m_frontendUuid);
        }
        m_frontendUuid = peerUuid;
        if (!m_frontendUuid.isNull()) {
            m_renderer->rendererAspect()->aspectManager()->changeArbiter()->registerObserver(this, m_frontendUuid, NodeUpdated);
            setXMin(peer->rect().x());
            setXMax(peer->rect().width());
            setYMin(peer->rect().y());
            setYMax(peer->rect().height());
        }
    }
}

float ViewportNode::xMin() const
{
    return m_xMin;
}

void ViewportNode::setXMin(float xMin)
{
    m_xMin = xMin;
}
float ViewportNode::yMin() const
{
    return m_yMin;
}

void ViewportNode::setYMin(float yMin)
{
    m_yMin = yMin;
}
float ViewportNode::xMax() const
{
    return m_xMax;
}

void ViewportNode::setXMax(float xMax)
{
    m_xMax = xMax;
}
float ViewportNode::yMax() const
{
    return m_yMax;
}

void ViewportNode::setYMax(float yMax)
{
    m_yMax = yMax;
}

void ViewportNode::sceneChangeEvent(const QSceneChangePtr &e)
{
    if (e->type() == NodeUpdated) {
        QScenePropertyChangePtr propertyChange = qSharedPointerCast<QScenePropertyChange>(e);
        if (propertyChange->propertyName() == QByteArrayLiteral("rect")) {
            QRectF rect = propertyChange->value().value<QRectF>();
            setXMin(rect.x());
            setYMin(rect.y());
            setXMax(rect.width());
            setYMax(rect.height());
        }
    }
}

} // Render

} // Qt3D

QT_END_NAMESPACE
