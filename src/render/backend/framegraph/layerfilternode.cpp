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

#include "layerfilternode.h"
#include "renderer.h"
#include "rendereraspect.h"
#include "qlayerfilter.h"
#include <Qt3DCore/qchangearbiter.h>
#include <Qt3DCore/qscenepropertychange.h>
#include <Qt3DCore/qaspectmanager.h>


QT_BEGIN_NAMESPACE

namespace Qt3D {

namespace Render {

LayerFilterNode::LayerFilterNode(FrameGraphNode *parent)
    : FrameGraphNode(FrameGraphNode::LayerFilter, parent)
    , m_renderer(Q_NULLPTR)
    , m_peer(Q_NULLPTR)
{
}

void LayerFilterNode::setRenderer(Renderer *renderer)
{
    m_renderer = renderer;
}

void LayerFilterNode::setPeer(QLayerFilter *peer)
{
    if (m_peer != peer) {
        QChangeArbiter *arbiter = m_renderer->rendererAspect()->aspectManager()->changeArbiter();
        if (m_peer)
            arbiter->unregisterObserver(this, m_peer);
        m_peer = peer;
        if (m_peer)
            arbiter->registerObserver(this, m_peer);
    }
}

void LayerFilterNode::sceneChangeEvent(const QSceneChangePtr &e)
{
    if (e->m_type == ComponentUpdated) {
        QScenePropertyChangePtr propertyChange = qSharedPointerCast<QScenePropertyChange>(e);
        if (propertyChange->m_propertyName == QByteArrayLiteral("layers"))
            setLayers(propertyChange->m_value.value<QStringList>());
    }
}

QStringList LayerFilterNode::layers() const
{
    return m_layers;
}

void LayerFilterNode::setLayers(const QStringList &list)
{
    m_layers = list;
}

} // Render

} //Qt3D

QT_END_NAMESPACE
