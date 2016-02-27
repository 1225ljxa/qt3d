/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Copyright (C) 2016 The Qt Company Ltd and/or its subsidiary(-ies).
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

#include "qalphatest.h"
#include "qrenderstate_p.h"
#include <private/qnode_p.h>
#include <Qt3DCore/qscenepropertychange.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

class QAlphaTestPrivate : public QRenderStatePrivate
{
public:
    QAlphaTestPrivate()
        : QRenderStatePrivate(QRenderState::AlphaTest)
        , m_alphaFunction(QAlphaTest::Never)
        , m_referenceValue(0.0f)
    {
    }

    Q_DECLARE_PUBLIC(QAlphaTest)
    QAlphaTest::AlphaFunction m_alphaFunction;
    float m_referenceValue;
};

QAlphaTest::QAlphaTest(QNode *parent)
    : QRenderState(*new QAlphaTestPrivate, parent)
{
}

QAlphaTest::~QAlphaTest()
{
    QNode::cleanup();
}

void QAlphaTest::copy(const QNode *ref)
{
    QRenderState::copy(ref);
    const QAlphaTest *refState = static_cast<const QAlphaTest*>(ref);
    d_func()->m_alphaFunction = refState->d_func()->m_alphaFunction;
    d_func()->m_referenceValue = refState->d_func()->m_referenceValue;
}

QAlphaTest::AlphaFunction QAlphaTest::alphaFunction() const
{
    Q_D(const QAlphaTest);
    return d->m_alphaFunction;
}

void QAlphaTest::setAlphaFunction(QAlphaTest::AlphaFunction alphaFunction)
{
    Q_D(QAlphaTest);
    if (d->m_alphaFunction != alphaFunction) {
        d->m_alphaFunction = alphaFunction;
        emit alphaFunctionChanged(alphaFunction);
    }
}

float QAlphaTest::referenceValue() const
{
    Q_D(const QAlphaTest);
    return d->m_referenceValue;
}

void QAlphaTest::setReferenceValue(float referenceValue)
{
    Q_D(QAlphaTest);
    if (d->m_referenceValue != referenceValue) {
        d->m_referenceValue = referenceValue;
        emit referenceValueChanged(referenceValue);
    }
}

} // namespace Qt3DRender

QT_END_NAMESPACE
