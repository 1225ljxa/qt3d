/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
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

#include "qviewport.h"
#include "qviewport_p.h"

#include <Qt3DCore/qscenepropertychange.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

QViewportPrivate::QViewportPrivate()
    : QFrameGraphNodePrivate()
    , m_normalizedRect(QRectF(0.0f, 0.0f, 1.0f, 1.0f))
{
}

void QViewport::copy(const QNode *ref)
{
    QFrameGraphNode::copy(ref);
    const QViewport *viewport = static_cast<const QViewport*>(ref);
    d_func()->m_normalizedRect = viewport->d_func()->m_normalizedRect;
}

QViewport::QViewport(QNode *parent)
    : QFrameGraphNode(*new QViewportPrivate, parent)
{
}

QViewport::~QViewport()
{
    QNode::cleanup();
}

/*! \internal */
QViewport::QViewport(QViewportPrivate &dd, QNode *parent)
    : QFrameGraphNode(dd, parent)
{
}

QRectF QViewport::normalizedRect() const
{
    Q_D(const QViewport);
    return d->m_normalizedRect;
}

void QViewport::setNormalizedRect(const QRectF &normalizedRect)
{
    Q_D(QViewport);
    if (normalizedRect != d->m_normalizedRect) {
        d->m_normalizedRect = normalizedRect;
        emit normalizedRectChanged(normalizedRect);
    }
}

} // namespace Qt3DRender

QT_END_NAMESPACE
