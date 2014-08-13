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

#include "qtranslatetransform.h"
#include "qtranslatetransform_p.h"

QT_BEGIN_NAMESPACE

namespace Qt3D {


QTranslateTransformPrivate::QTranslateTransformPrivate(QTranslateTransform *qq)
    : QAbstractTransformPrivate(qq)
{
}

QTranslateTransform::QTranslateTransform(QNode *parent)
    : QAbstractTransform(*new QTranslateTransformPrivate(this), parent)
{
}

void QTranslateTransform::copy(const QNode *ref)
{
    Q_D(QTranslateTransform);
    QAbstractTransform::copy(ref);
    const QTranslateTransform *transform = qobject_cast<const QTranslateTransform *>(ref);
    if (transform != Q_NULLPTR) {
        d->m_translation = transform->translation();
    }
}


QTranslateTransform::QTranslateTransform(QTranslateTransformPrivate &dd, QNode *parent)
    : QAbstractTransform(dd, parent)
{
}

QTranslateTransform *QTranslateTransform::doClone(QNode *clonedParent) const
{
    return new QTranslateTransform(clonedParent);
}


float QTranslateTransform::dx() const
{
    Q_D(const QTranslateTransform);
    return d->m_translation.x();
}

float QTranslateTransform::dy() const
{
    Q_D(const QTranslateTransform);
    return d->m_translation.y();
}

float QTranslateTransform::dz() const
{
    Q_D(const QTranslateTransform);
    return d->m_translation.z();
}

QVector3D QTranslateTransform::translation() const
{
    Q_D(const QTranslateTransform);
    return d->m_translation;
}

QMatrix4x4 QTranslateTransform::matrix() const
{
    Q_D(const QTranslateTransform);
    QMatrix4x4 r;
    r.translate(d->m_translation);
    return r;
}

void QTranslateTransform::setDx(float arg)
{
    Q_D(QTranslateTransform);
    if (arg != d->m_translation.x()) {
        d->m_translation.setX(arg);
        emit translateChanged();
        emit transformUpdated();
    }
}

void QTranslateTransform::setDy(float arg)
{
    Q_D(QTranslateTransform);
    if (arg != d->m_translation.y()) {
        d->m_translation.setY(arg);
        emit translateChanged();
        emit transformUpdated();
    }
}

void QTranslateTransform::setDz(float arg)
{
    Q_D(QTranslateTransform);
    if (arg != d->m_translation.z()) {
        d->m_translation.setZ(arg);
        emit translateChanged();
        emit transformUpdated();
    }
}

void QTranslateTransform::setTranslation(QVector3D arg)
{
    Q_D(QTranslateTransform);
    if (d->m_translation != arg) {
        d->m_translation = arg;
        emit translateChanged();
    }
}

} // namespace Qt3D

QT_END_NAMESPACE
