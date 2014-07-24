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

#include "rendertechnique.h"
#include "rendershader.h"

#include <qrenderpass.h>
#include <qtechnique.h>
#include "rendereraspect.h"
#include "renderer.h"
#include "rendercriterion.h"
#include "techniquecriterionmanager.h"
#include "qopenglfilter.h"
#include <Qt3DCore/qaspectmanager.h>
#include <Qt3DCore/qchangearbiter.h>
#include <Qt3DCore/qscenepropertychange.h>

#include <QDebug>

QT_BEGIN_NAMESPACE

namespace Qt3D {
namespace Render {

RenderTechnique::RenderTechnique() :
    m_renderer(Q_NULLPTR),
    m_peer(Q_NULLPTR),
    m_passCount(0),
    m_openglFilter(new QOpenGLFilter())
{
}

RenderTechnique::~RenderTechnique()
{
    cleanup();
}

void RenderTechnique::cleanup()
{
    if (m_renderer != Q_NULLPTR && m_peer != Q_NULLPTR) {
        m_renderer->rendererAspect()->aspectManager()->changeArbiter()->unregisterObserver(this, m_peer);
        delete m_openglFilter;
    }
}

void RenderTechnique::setRenderer(Renderer *renderer)
{
    m_renderer = renderer;
    m_parameterPack.setRenderer(m_renderer);
}

void RenderTechnique::setPeer(QTechnique *peer)
{
    if (m_peer != peer) {
        QChangeArbiter *arbiter = m_renderer->rendererAspect()->aspectManager()->changeArbiter();
        arbiter->unregisterObserver(this, m_peer);
        m_peer = peer;
        if (m_peer) {
            arbiter->registerObserver(this, m_peer);
            m_parameterPack.clear();
            Q_FOREACH (QParameter *p, m_peer->parameters())
                m_parameterPack.appendParameter(p);

            QOpenGLFilter *peerFilter = peer->openGLFilter();
            // Copy OpenGLFilter info from frontend OpenGLFilter
            // QObject doesn't allow copying directly
            m_openglFilter->setMinorVersion(peerFilter->minorVersion());
            m_openglFilter->setMajorVersion(peerFilter->majorVersion());
            m_openglFilter->setExtensions(peerFilter->extensions());
            m_openglFilter->setVendor(peerFilter->vendor());
            m_openglFilter->setApi(peerFilter->api());
            m_openglFilter->setProfile(peerFilter->profile());
        }
    }
}

QTechnique *RenderTechnique::peer() const
{
    return m_peer;
}

QParameter *RenderTechnique::parameterByName(QString name) const
{
    foreach (QParameter* p, m_peer->parameters()) {
        if (p->name() == name)
            return p;
    }

    qWarning() << Q_FUNC_INFO << "couldn't find parameter:" << name
               << "in technique" << m_peer->objectName();
    return NULL;
}

void RenderTechnique::sceneChangeEvent(const QSceneChangePtr &e)
{
    QScenePropertyChangePtr propertyChange = qSharedPointerCast<QScenePropertyChange>(e);
    switch (e->type()) {

    case ComponentAdded: {
        if (propertyChange->propertyName() == QByteArrayLiteral("pass")) {
            // This will be filled when we have a proper backend RenderPass class
        }
        else if (propertyChange->propertyName() == QByteArrayLiteral("parameter")) {
            m_parameterPack.appendParameter(propertyChange->value().value<QParameter*>());
        }
        else if (propertyChange->propertyName() == QByteArrayLiteral("criterion")) {
            QTechniqueCriterion *crit = propertyChange->value().value<QTechniqueCriterion *>();
            HTechniqueCriterion critHandle = m_renderer->techniqueCriterionManager()->lookupHandle(crit->uuid());
            if (critHandle.isNull()) {
                critHandle = m_renderer->techniqueCriterionManager()->getOrAcquireHandle(crit->uuid());
                RenderCriterion *renderCriterion = m_renderer->techniqueCriterionManager()->data(critHandle);
                renderCriterion->setRenderer(m_renderer);
                renderCriterion->setPeer(crit);
            }
            if (!m_criteriaList.contains(critHandle))
                m_criteriaList.append(critHandle);
        }
        break;

    case ComponentRemoved: {
            if (propertyChange->propertyName() == QByteArrayLiteral("pass")) {
                // See above
            }
            else if (propertyChange->propertyName() == QByteArrayLiteral("parameter")) {
                m_parameterPack.removeParameter(propertyChange->value().value<QParameter*>());
            }
            else if (propertyChange->propertyName() == QByteArrayLiteral("criterion")) {
                m_criteriaList.removeOne(m_renderer->techniqueCriterionManager()->lookupHandle(propertyChange->value().toUuid()));
            }
        }
            break;

        default:
            break;
        }
    }
}

const QHash<QString, QVariant> RenderTechnique::parameters() const
{
    return m_parameterPack.namedValues();
}

QList<HTechniqueCriterion> RenderTechnique::criteria() const
{
    return m_criteriaList;
}

QOpenGLFilter *RenderTechnique::openGLFilter() const
{
    return m_openglFilter;
}

} // namespace Render
} // namespace Qt3D

QT_END_NAMESPACE
