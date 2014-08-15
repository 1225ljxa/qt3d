/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
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

#include "qblendstate.h"
#include <Qt3DCore/qscenepropertychange.h>
#include <private/qnode_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

class QBlendStatePrivate : public QNodePrivate
{
public:
    QBlendStatePrivate(QBlendState *qq)
        : QNodePrivate(qq)
        , m_srcRGB(QBlendState::Zero)
        , m_srcAlpha(QBlendState::Zero)
        , m_dstRGB(QBlendState::Zero)
        , m_dstAlpha(QBlendState::Zero)
    {
    }

    Q_DECLARE_PUBLIC(QBlendState)

    QBlendState::Blending m_srcRGB;
    QBlendState::Blending m_srcAlpha;
    QBlendState::Blending m_dstRGB;
    QBlendState::Blending m_dstAlpha;
};

QBlendState::QBlendState(QNode *parent)
    : QRenderState(*new QBlendStatePrivate(this), parent)
{
}

void QBlendState::copy(const QNode *ref)
{
    QRenderState::copy(ref);
    Q_D(QBlendState);
    const QBlendState *refState = qobject_cast<const QBlendState *>(ref);
    if (refState != Q_NULLPTR) {
        d->m_srcRGB = refState->srcRGB();
        d->m_srcAlpha = refState->srcAlpha();
        d->m_dstAlpha = refState->dstAlpha();
        d->m_dstRGB = refState->dstRGB();
    }
}

QBlendState::Blending QBlendState::srcRGB() const
{
    Q_D(const QBlendState);
    return d->m_srcRGB;
}

void QBlendState::setSrcRGB(QBlendState::Blending srcRGB)
{
    Q_D(QBlendState);
    if (d->m_srcRGB != srcRGB) {
        d->m_srcRGB = srcRGB;
        emit srcRGBChanged();
        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr propertyChange(new QScenePropertyChange(NodeUpdated, this));
            propertyChange->setPropertyName(QByteArrayLiteral("srcRGB"));
            propertyChange->setValue(d->m_srcRGB);
            notifyObservers(propertyChange);
        }
    }
}

QBlendState::Blending QBlendState::dstRGB() const
{
    Q_D(const QBlendState);
    return d->m_dstRGB;
}

void QBlendState::setDstRGB(QBlendState::Blending dstRGB)
{
    Q_D(QBlendState);
    if (d->m_dstRGB != dstRGB) {
        d->m_dstRGB = dstRGB;
        emit dstRGBChanged();
        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr propertyChange(new QScenePropertyChange(NodeUpdated, this));
            propertyChange->setPropertyName(QByteArrayLiteral("dstRGB"));
            propertyChange->setValue(d->m_dstRGB);
            notifyObservers(propertyChange);
        }
    }
}

QBlendState::Blending QBlendState::srcAlpha() const
{
    Q_D(const QBlendState);
    return d->m_srcAlpha;
}

void QBlendState::setSrcAlpha(QBlendState::Blending srcAlpha)
{
    Q_D(QBlendState);
    if (d->m_srcAlpha != srcAlpha) {
        d->m_srcAlpha = srcAlpha;
        emit srcAlphaChanged();
        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr propertyChange(new QScenePropertyChange(NodeUpdated, this));
            propertyChange->setPropertyName(QByteArrayLiteral("srcAlpha"));
            propertyChange->setValue(d->m_srcAlpha);
            notifyObservers(propertyChange);
        }
    }
}

QBlendState::Blending QBlendState::dstAlpha() const
{
    Q_D(const QBlendState);
    return d->m_dstAlpha;
}

void QBlendState::setDstAlpha(QBlendState::Blending dstAlpha)
{
    Q_D(QBlendState);
    if (d->m_dstAlpha != dstAlpha) {
        d->m_dstAlpha = dstAlpha;
        emit dstAlphaChanged();
        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr propertyChange(new QScenePropertyChange(NodeUpdated, this));
            propertyChange->setPropertyName(QByteArrayLiteral("dstAlpha"));
            propertyChange->setValue(d->m_dstAlpha);
            notifyObservers(propertyChange);
        }
    }
}

QNode *QBlendState::doClone(QNode *clonedParent) const
{
    return new QBlendState(clonedParent);
}

} // Qt3D

QT_END_NAMESPACE
