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

#include "qstenciltest.h"
#include <private/qnode_p.h>
#include <Qt3DCore/qscenepropertychange.h>


QT_BEGIN_NAMESPACE

namespace Qt3D {

class QStencilTestPrivate : public QNodePrivate
{
public:
    QStencilTestPrivate(QStencilTest *qq)
        : QNodePrivate(qq)
        , m_mask(0)
        , m_func(QStencilTest::Never)
        , m_faceMode(QStencilTest::FrontAndBack)
    {
    }

    Q_DECLARE_PUBLIC(QStencilTest)
    uint m_mask;
    QStencilTest::StencilFunc m_func;
    QStencilTest::StencilFaceMode m_faceMode;
};

QStencilTest::QStencilTest(QNode *parent)
    : QRenderState(*new QStencilTestPrivate(this), parent)
{
}

void QStencilTest::copy(const QNode *ref)
{
    QRenderState::copy(ref);
    Q_D(QStencilTest);
    const QStencilTest *refState = qobject_cast<const QStencilTest *>(ref);
    if (refState != Q_NULLPTR) {
        d->m_mask = refState->mask();
        d->m_faceMode = refState->faceMode();
        d->m_func = refState->func();
    }
}

uint QStencilTest::mask() const
{
    Q_D(const QStencilTest);
    return d->m_mask;
}

void QStencilTest::setMask(uint mask)
{
    Q_D(QStencilTest);
    if (d->m_mask != mask) {
        d->m_mask = mask;
        emit maskChanged();
        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr propertyChange(new QScenePropertyChange(NodeUpdated, this));
            propertyChange->setPropertyName(QByteArrayLiteral("mask"));
            propertyChange->setValue(d->m_mask);
            notifyObservers(propertyChange);
        }
    }
}

QStencilTest::StencilFaceMode QStencilTest::faceMode() const
{
    Q_D(const QStencilTest);
    return d->m_faceMode;
}

void QStencilTest::setFaceMode(QStencilTest::StencilFaceMode mode)
{
    Q_D(QStencilTest);
    if (d->m_faceMode != mode) {
        d->m_faceMode = mode;
        emit faceModeChanged();
        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr propertyChange(new QScenePropertyChange(NodeUpdated, this));
            propertyChange->setPropertyName(QByteArrayLiteral("faceMode"));
            propertyChange->setValue(d->m_faceMode);
            notifyObservers(propertyChange);
        }
    }
}

QStencilTest::StencilFunc QStencilTest::func() const
{
    Q_D(const QStencilTest);
    return d->m_func;
}

void QStencilTest::setFunc(QStencilTest::StencilFunc func)
{
    Q_D(QStencilTest);
    if (d->m_func != func) {
        d->m_func = func;
        emit funcChanged();
        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr propertyChange(new QScenePropertyChange(NodeUpdated, this));
            propertyChange->setPropertyName(QByteArrayLiteral("func"));
            propertyChange->setValue(d->m_func);
            notifyObservers(propertyChange);
        }
    }
}

QNode *QStencilTest::doClone(QNode *clonedParent) const
{
    return new QStencilTest(clonedParent);
}

} // Qt3D

QT_END_NAMESPACE
