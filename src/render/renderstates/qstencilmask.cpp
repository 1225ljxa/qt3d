/****************************************************************************
**
** Copyright (C) 2015 Paul Lemire
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

#include "qstencilmask.h"
#include <Qt3DRender/private/qrenderstate_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

class QStencilMaskPrivate : public QRenderStatePrivate
{
public:
    QStencilMaskPrivate()
        : QRenderStatePrivate(QRenderState::StencilMask)
        , m_frontOutputMask(0)
        , m_backOutputMask(0)
    {}

    uint m_frontOutputMask;
    uint m_backOutputMask;
};

/*!
 * QStencilMask::QStencilMask
 */

QStencilMask::QStencilMask(QNode *parent)
    : QRenderState(*new QStencilMaskPrivate(), parent)
{
}

QStencilMask::~QStencilMask()
{
    QNode::cleanup();
}

void QStencilMask::setFrontOutputMask(uint mask)
{
    Q_D(QStencilMask);
    if (d->m_frontOutputMask != mask) {
        d->m_frontOutputMask = mask;
        Q_EMIT frontOutputMaskChanged(mask);
    }
}

void QStencilMask::setBackOutputMask(uint mask)
{
    Q_D(QStencilMask);
    if (d->m_backOutputMask != mask) {
        d->m_backOutputMask = mask;
        Q_EMIT backOutputMaskChanged(mask);
    }
}

uint QStencilMask::frontOutputMask() const
{
    Q_D(const QStencilMask);
    return d->m_frontOutputMask;
}

uint QStencilMask::backOutputMask() const
{
    Q_D(const QStencilMask);
    return d->m_backOutputMask;
}

void QStencilMask::copy(const QNode *ref)
{
    QRenderState::copy(ref);
    const QStencilMask *otherRef = static_cast<const QStencilMask *>(ref);
    d_func()->m_frontOutputMask = otherRef->frontOutputMask();
    d_func()->m_backOutputMask = otherRef->backOutputMask();
}

} // namespace Qt3DRender

QT_END_NAMESPACE
