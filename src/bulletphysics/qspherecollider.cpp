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

#include "qspherecollider.h"
#include "qspherecollider_p.h"

QT_BEGIN_NAMESPACE

namespace Qt3D {
namespace BulletPhysics {

QSphereColliderPrivate::QSphereColliderPrivate(QSphereCollider *qq)
    : QAbstractColliderPrivate(qq)
    , m_center()
    , m_radius(1.0f)
{
}


QSphereCollider::QSphereCollider(Qt3D::QNode *parent)
    : QAbstractCollider(*new QSphereColliderPrivate(this), parent)
{
}

QSphereCollider::QSphereCollider(QSphereColliderPrivate &dd, QNode *parent)
    : QAbstractCollider(dd, parent)
{
}

void QSphereCollider::copy(const QNode *ref)
{
    QAbstractCollider::copy(ref);
    const QSphereCollider *other = static_cast<const QSphereCollider*>(ref);
    d_func()->m_center = other->d_func()->m_center;
    d_func()->m_radius = other->d_func()->m_radius;
}

void QSphereCollider::setCenter(const QVector3D &center)
{
    Q_D(QSphereCollider);
    if (!qFuzzyCompare(d->m_center, center)) {
        setCenterX(center.x());
        setCenterY(center.y());
        setCenterZ(center.z());
        emit centerChanged();
    }
}

QVector3D QSphereCollider::center() const
{
    Q_D(const QSphereCollider);
    return d->m_center;
}

void QSphereCollider::setRadius(float radius)
{
    Q_D(QSphereCollider);
    if (d->m_radius != radius) {
        d->m_radius = radius;
        emit radiusChanged();
    }
}

float QSphereCollider::radius() const
{
    Q_D(const QSphereCollider);
    return d->m_radius;
}

void QSphereCollider::setCenterX(float x)
{
    Q_D(QSphereCollider);
    if (!qFuzzyCompare(d->m_center.x(), x)) {
        d->m_center.setX(x);
        emit centerXChanged();
    }
}

float QSphereCollider::centerX() const
{
    Q_D(const QSphereCollider);
    return d->m_center.x();
}

void QSphereCollider::setCenterY(float y)
{
    Q_D(QSphereCollider);
    if (!qFuzzyCompare(d->m_center.y(), y)) {
        d->m_center.setY(y);
        emit centerYChanged();
    }
}

float QSphereCollider::centerY() const
{
    Q_D(const QSphereCollider);
    return d->m_center.y();
}

void QSphereCollider::setCenterZ(float z)
{
    Q_D(QSphereCollider);
    if (!qFuzzyCompare(d->m_center.z(), z)) {
        d->m_center.setZ(z);
        emit centerZChanged();
    }
}

float QSphereCollider::centerZ() const
{
    Q_D(const QSphereCollider);
    return d->m_center.z();
}

} // namespace BulletPhysics
} // namespace Qt3D

QT_END_NAMESPACE
