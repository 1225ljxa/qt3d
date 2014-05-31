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

#include <QVariant>
#include "rendereffect.h"
#include "rendereraspect.h"
#include <Qt3DCore/qaspectmanager.h>
#include <Qt3DCore/qabstracteffect.h>
#include <Qt3DCore/qabstracttechnique.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

namespace Render {

RenderEffect::RenderEffect()
    : m_rendererAspect(Q_NULLPTR)
    , m_peer(Q_NULLPTR)
{
}

RenderEffect::~RenderEffect()
{
}

void RenderEffect::setPeer(QAbstractEffect *effect)
{
    if (effect != m_peer) {
        QChangeArbiter *arbiter = m_rendererAspect->aspectManager()->changeArbiter();
        m_techniques.clear();
        if (m_peer)
            arbiter->unregisterObserver(this, m_peer);
        m_peer = effect;
        if (m_peer) {
            arbiter->registerObserver(this, m_peer, ComponentUpdated);
            m_techniques.append(m_peer->techniques());
        }
    }
}

void RenderEffect::setRendererAspect(RendererAspect *rendererAspect)
{
    m_rendererAspect = rendererAspect;
}

void RenderEffect::sceneChangeEvent(const QSceneChangePtr &e)
{
    QScenePropertyChangePtr propertyChange = qSharedPointerCast<QScenePropertyChange>(e);
    QString propertyName = QString::fromLatin1(propertyChange->m_propertyName);
    QVariant propertyValue = propertyChange->m_value;
    switch (e->m_type) {

    case ComponentAdded:
        if (propertyName == QStringLiteral("technique")) {
            m_techniques.append(propertyValue.value<QAbstractTechnique *>());
        }
        break;

    case ComponentRemoved:
        if (propertyName == QStringLiteral("technique")) {
            m_techniques.removeOne(propertyValue.value<QAbstractTechnique *>());
        }
        break;

    default :
        break;
    }
}

QList<QAbstractTechnique *> RenderEffect::techniques() const
{
    return m_techniques;
}

} // Render

} // Qt3D

QT_END_NAMESPACE
