/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qtransform.h"
#include "qtransform_p.h"
#include "qabstracttransform_p.h"
#include "qmath3d_p.h"

#include <Qt3DCore/qscenepropertychange.h>

QT_BEGIN_NAMESPACE

namespace Qt3DCore {

/*!
    \class Qt3DCore::QTransformPrivate
    \internal
*/
QTransformPrivate::QTransformPrivate()
    : QComponentPrivate()
    , m_transformsDirty(false)
    , m_rotation()
    , m_scale(1.0f, 1.0f, 1.0f)
    , m_translation()
{
}

void QTransformPrivate::_q_transformDestroyed(QObject *obj)
{
    QAbstractTransform *transform = static_cast<QAbstractTransform *>(obj);
    if (m_transforms.removeOne(transform)) {
        emit q_func()->transformsChanged();
        _q_update();
    }
}

void QTransformPrivate::_q_update()
{
    if (!m_transformsDirty)
        m_transformsDirty = true;
    emit q_func()->matrixChanged();
}

QMatrix4x4 QTransformPrivate::applyTransforms() const
{
    QMatrix4x4 matrix;
    Q_FOREACH (const QAbstractTransform *t, m_transforms)
        matrix = t->transformMatrix() * matrix;
    return matrix;
}


QTransform::QTransform(QNode *parent)
    : QComponent(*new QTransformPrivate, parent)
{
}

QTransform::QTransform(QList<QAbstractTransform *> transforms, QNode *parent)
    : QComponent(*new QTransformPrivate, parent)
{
    Q_FOREACH (QAbstractTransform *t, transforms)
        addTransform(t);
}

QTransform::QTransform(QAbstractTransform *transform, QNode *parent)
    : QComponent(*new QTransformPrivate, parent)
{
    addTransform(transform);
}

/*! \internal */
QTransform::QTransform(QTransformPrivate &dd, QNode *parent)
    : QComponent(dd, parent)
{
}

QTransform::~QTransform()
{
    QNode::cleanup();
    Q_D(QTransform);
    // boost destruction by avoiding _q_update()-s
    d->m_transforms.clear();
}

void QTransform::copy(const QNode *ref)
{
    QComponent::copy(ref);
    const QTransform *transform = static_cast<const QTransform *>(ref);
    // We need to copy the matrix with all the pending
    // transformations applied
    d_func()->m_matrix = transform->matrix();
    d_func()->m_rotation = transform->rotation();
    d_func()->m_scale = transform->scale3D();
    d_func()->m_translation = transform->translation();
}

QList<QAbstractTransform *> QTransform::transforms() const
{
    Q_D(const QTransform);
    return d->m_transforms;
}

void QTransform::addTransform(QAbstractTransform *transform)
{
    Q_D(QTransform);
    if (transform == Q_NULLPTR || d->m_transforms.contains(transform))
        return;
    d->m_transforms.append(transform);
    QObject::connect(transform, SIGNAL(transformMatrixChanged()), this, SLOT(_q_update()));
    QObject::connect(transform, SIGNAL(destroyed(QObject*)), this, SLOT(_q_transformDestroyed(QObject*)));
    emit transformsChanged();
    d->_q_update();
}

void QTransform::removeTransform(QAbstractTransform *transform)
{
    Q_D(QTransform);
    if (!d->m_transforms.removeOne(transform))
        return;
    QObject::disconnect(transform, SIGNAL(transformMatrixChanged()), this, SLOT(_q_update()));
    QObject::disconnect(transform, SIGNAL(destroyed(QObject*)), this, SLOT(_q_transformDestroyed(QObject*)));
    emit transformsChanged();
    d->_q_update();
}

void QTransform::setMatrix(const QMatrix4x4 &m)
{
    Q_D(QTransform);
    if (m != matrix()) {
        d->m_matrix = m;
        d->m_matrixDirty = false;

        QVector3D s;
        QVector3D t;
        QQuaternion r;
        decomposeQMatrix4x4(m, t, r, s);
        d->m_scale = s;
        d->m_rotation = r;
        d->m_translation = t;
        emit scale3DChanged();
        emit rotationChanged();
        emit translationChanged();

        const bool wasBlocked = blockNotifications(true);
        emit matrixChanged();
        blockNotifications(wasBlocked);
    }
}

QMatrix4x4 QTransform::matrix() const
{
    Q_D(const QTransform);
    if (d->m_matrixDirty) {
        composeQMatrix4x4(d->m_translation, d->m_rotation, d->m_scale, d->m_matrix);
        d->m_matrixDirty = false;
    }
    return d->m_matrix;
}

void QTransform::setScale3D(const QVector3D &scale)
{
    Q_D(QTransform);
    if (scale != d->m_scale) {
        d->m_scale = scale;
        emit scale3DChanged();
    }
}

QVector3D QTransform::scale3D() const
{
    Q_D(const QTransform);
    return d->m_scale;
}

void QTransform::setScale(float scale)
{
    Q_D(QTransform);
    if (scale != d->m_scale.x()) {
        setScale3D(QVector3D(scale, scale, scale));
        const bool wasBlocked = blockNotifications(true);
        emit scaleChanged();
        blockNotifications(wasBlocked);
    }
}

float QTransform::scale() const
{
    Q_D(const QTransform);
    return d->m_scale.x();
}

void QTransform::setRotation(const QQuaternion &rotation)
{
    Q_D(QTransform);
    if (rotation != d->m_rotation) {
        d->m_rotation = rotation;
        emit rotationChanged();
    }
}

QQuaternion QTransform::rotation() const
{
    Q_D(const QTransform);
    return d->m_rotation;
}

void QTransform::setTranslation(const QVector3D &translation)
{
    Q_D(QTransform);
    if (translation != d->m_translation) {
        d->m_translation = translation;
        emit translationChanged();
    }
}

QVector3D QTransform::translation() const
{
    Q_D(const QTransform);
    return d->m_translation;
}

QQuaternion QTransform::fromAxisAndAngle(const QVector3D &axis, float angle)
{
    return QQuaternion::fromAxisAndAngle(axis, angle);
}

QQuaternion QTransform::fromAxisAndAngle(float x, float y, float z, float angle)
{
    return QQuaternion::fromAxisAndAngle(x, y, z, angle);
}

QQuaternion QTransform::fromAxesAndAngles(const QVector3D &axis1, float angle1,
                                          const QVector3D &axis2, float angle2)
{
    const QQuaternion q1 = QQuaternion::fromAxisAndAngle(axis1, angle1);
    const QQuaternion q2 = QQuaternion::fromAxisAndAngle(axis2, angle2);
    return q2 * q1;
}

QQuaternion QTransform::fromAxesAndAngles(const QVector3D &axis1, float angle1,
                                          const QVector3D &axis2, float angle2,
                                          const QVector3D &axis3, float angle3)
{
    const QQuaternion q1 = QQuaternion::fromAxisAndAngle(axis1, angle1);
    const QQuaternion q2 = QQuaternion::fromAxisAndAngle(axis2, angle2);
    const QQuaternion q3 = QQuaternion::fromAxisAndAngle(axis3, angle3);
    return q3 * q2 * q1;
}

QQuaternion QTransform::fromEulerAngles(const QVector3D &eulerAngles)
{
    return QQuaternion::fromEulerAngles(eulerAngles);
}

QQuaternion QTransform::fromEulerAngles(float pitch, float yaw, float roll)
{
    return QQuaternion::fromEulerAngles(pitch, yaw, roll);
}

} // namespace Qt3DCore

QT_END_NAMESPACE

#include "moc_qtransform.cpp"
