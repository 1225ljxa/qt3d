/****************************************************************************
**
** Copyright (C) 2015 Klaralvdalens Datakonsult AB (KDAB).
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

#include "qaxissetting.h"
#include <Qt3DCore/private/qnode_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3DInput {

class QAxisSettingPrivate : public Qt3DCore::QNodePrivate
{
public:
    QAxisSettingPrivate()
        : Qt3DCore::QNodePrivate()
        , m_deadZone(0.0f)
        , m_axes()
        , m_filter(false)
    {}

    float m_deadZone;
    QVariantList m_axes;
    bool m_filter;
};

QAxisSetting::QAxisSetting(Qt3DCore::QNode *parent)
    : QNode(*new QAxisSettingPrivate(), parent)
{
}

QAxisSetting::~QAxisSetting()
{
    QNode::cleanup();
}

QVariantList QAxisSetting::axes() const
{
    Q_D(const QAxisSetting);
    return d->m_axes;
}

float QAxisSetting::deadZone() const
{
    Q_D(const QAxisSetting);
    return d->m_deadZone;
}

bool QAxisSetting::isFilterEnabled() const
{
    Q_D(const QAxisSetting);
    return d->m_filter;
}

void QAxisSetting::setDeadZone(float deadZone)
{
    Q_D(QAxisSetting);
    if (d->m_deadZone == deadZone)
        return;

    d->m_deadZone = deadZone;
    emit deadZoneChanged(deadZone);
}

void QAxisSetting::setAxes(const QVariantList &axes)
{
    Q_D(QAxisSetting);
    if (d->m_axes == axes)
        return;

    d->m_axes = axes;
    emit axesChanged(axes);
}

void QAxisSetting::setFilterEnabled(bool enabled)
{
    Q_D(QAxisSetting);
    if (d->m_filter == enabled)
        return;

    d->m_filter = enabled;
    emit filterChanged(enabled);
}

void QAxisSetting::copy(const Qt3DCore::QNode *ref)
{
    QNode::copy(ref);
    const QAxisSetting *setting = static_cast<const QAxisSetting *>(ref);
    d_func()->m_deadZone = setting->d_func()->m_deadZone;
    d_func()->m_axes = setting->d_func()->m_axes;
    d_func()->m_filter = setting->d_func()->m_filter;
}

} // namespace Qt3DInput

QT_END_NAMESPACE
