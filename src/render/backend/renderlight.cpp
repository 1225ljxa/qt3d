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

#include "renderlight.h"
#include "renderer.h"
#include "rendereraspect.h"
#include "qabstractlight.h"
#include <Qt3DCore/qchangearbiter.h>
#include <Qt3DCore/qaspectmanager.h>
#include <Qt3DCore/qscenepropertychange.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

namespace Render {

RenderLight::RenderLight()
    : m_renderer(Q_NULLPTR)
    , m_peer(Q_NULLPTR)
{
}

RenderLight::~RenderLight()
{
    if (m_peer)
        m_renderer->rendererAspect()->aspectManager()->changeArbiter()->unregisterObserver(this, m_peer);
}

void RenderLight::setPeer(QAbstractLight *peer)
{
    if (m_peer != peer) {
        QChangeArbiter *arbiter = m_renderer->rendererAspect()->aspectManager()->changeArbiter();
        if (m_peer) {
            arbiter->unregisterObserver(this, m_peer);
            m_lightProperties.clear();
        }
        m_peer = peer;
        if (m_peer) {
            arbiter->registerObserver(this, m_peer, ComponentUpdated);
            m_lightProperties = m_peer->lightProperties();
            m_lightProperties[QStringLiteral("color")] = m_peer->color();
            m_lightProperties[QStringLiteral("intensity")] = m_peer->intensity();
        }
    }
}

void RenderLight::setRenderer(Renderer *renderer)
{
    m_renderer = renderer;
}

QHash<QString, QVariant> RenderLight::lightProperties() const
{
    return m_lightProperties;
}

void RenderLight::sceneChangeEvent(const QSceneChangePtr &e)
{
    if (e->m_type == ComponentUpdated && e->m_subject.m_observable == m_peer) {
        QScenePropertyChangePtr propertyChange = qSharedPointerCast<QScenePropertyChange>(e);
        m_lightProperties[QString::fromUtf8(propertyChange->m_propertyName)] = propertyChange->m_value;
    }
}

} // Qt3D

} // Render

QT_END_NAMESPACE
