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

#include "renderrenderpass.h"
#include "renderer.h"
#include "rendereraspect.h"
#include "rendercriterion.h"
#include "criterionmanager.h"
#include <Qt3DCore/qaspectmanager.h>
#include <Qt3DCore/qchangearbiter.h>
#include <Qt3DCore/qscenepropertychange.h>
#include <Qt3DCore/qabstractshader.h>
#include <Qt3DRenderer/qparametermapper.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

namespace Render {

RenderRenderPass::RenderRenderPass()
    : m_renderer(Q_NULLPTR)
    , m_peer(Q_NULLPTR)
{
}

RenderRenderPass::~RenderRenderPass()
{
    cleanup();
}

void RenderRenderPass::cleanup()
{
    if (m_renderer != Q_NULLPTR && !m_passUuid.isNull())
        m_renderer->rendererAspect()->aspectManager()->changeArbiter()->unregisterObserver(this, m_passUuid);
}

void RenderRenderPass::setRenderer(Renderer *renderer)
{
    m_renderer = renderer;
}

void RenderRenderPass::setPeer(QRenderPass *peer)
{
    if (m_peer != peer) {
        QChangeArbiter *arbiter = m_renderer->rendererAspect()->aspectManager()->changeArbiter();
        if (!m_passUuid.isNull()) {
            arbiter->unregisterObserver(this, m_passUuid);
            m_passUuid = QUuid();
            m_shaderUuid = QUuid();
        }
        m_peer = peer;
        if (m_peer) {
            if (m_peer->shaderProgram() != Q_NULLPTR)
                m_shaderUuid = m_peer->shaderProgram()->uuid();
            m_passUuid = m_peer->uuid();
            arbiter->registerObserver(this, m_passUuid, NodeAdded|NodeRemoved);
            // TO DO -> Have backend classes for Bindings and Parameters so that we can easily monitor for updates
            m_bindings = m_peer->bindings();
            Q_FOREACH (QCriterion *c, m_peer->criteria())
                appendCriterion(c);
        }
    }
}

void RenderRenderPass::sceneChangeEvent(const QSceneChangePtr &e)
{
    QScenePropertyChangePtr propertyChange = qSharedPointerCast<QScenePropertyChange>(e);
    switch (e->type()) {

    case NodeAdded: {
        if (propertyChange->propertyName() == QByteArrayLiteral("criterion")) {
            appendCriterion(propertyChange->value().value<QCriterion *>());
        }
        else if (propertyChange->propertyName() == QByteArrayLiteral("shaderProgram")) {
            m_shaderUuid = propertyChange->value().toUuid();
        }
        break;
    }

    case NodeRemoved: {
        if (propertyChange->propertyName() == QByteArrayLiteral("criterion")) {
            removeCriterion(propertyChange->value().toUuid());
        }
        else if (propertyChange->propertyName() == QByteArrayLiteral("shaderProgram")) {
            m_shaderUuid = QUuid();
        }
        break;
    }

    default:
        break;
    }
}

QUuid RenderRenderPass::shaderProgram() const
{
    return m_shaderUuid;
}

QList<QParameterMapper *> RenderRenderPass::bindings() const
{
    return m_bindings;
}

QList<QUuid> RenderRenderPass::criteria() const
{
    return m_criteriaList;
}

QUuid RenderRenderPass::renderPassUuid() const
{
    return m_passUuid;
}

void RenderRenderPass::appendCriterion(QCriterion *criterion)
{
    if (!m_criteriaList.contains(criterion->uuid()))
        m_criteriaList.append(criterion->uuid());
}

void RenderRenderPass::removeCriterion(const QUuid &criterionId)
{
    m_criteriaList.removeOne(criterionId);
}

} // Render

} // Qt3D

QT_END_NAMESPACE
