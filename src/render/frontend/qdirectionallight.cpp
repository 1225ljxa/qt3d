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

#include "qdirectionallight.h"
#include "qdirectionallight_p.h"
#include <Qt3DCore/qscenepropertychange.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

/*!
 *
 * Expected Shader struct
 *
 * \code
 *
 * struct DirectionalLight
 * {
 *  vec3 position;
 *  vec3 direction;
 *  vec4 color;
 *  float intensity;
 * };
 *
 * uniform DirectionalLight directionalLights[10];
 *
 * \endcode
 */

QDirectionalLightPrivate::QDirectionalLightPrivate(QDirectionalLight *qq)
    : QAbstractLightPrivate(qq)
{
}

QDirectionalLight::QDirectionalLight(QNode *parent)
    : QAbstractLight(*new QDirectionalLightPrivate(this), parent)
{
}

void QDirectionalLight::copy(const QNode *ref)
{
    Q_D(QDirectionalLight);
    QAbstractLight::copy(ref);
    const QDirectionalLight *light = qobject_cast<const QDirectionalLight *>(ref);
    if (ref != Q_NULLPTR) {
        d->m_direction = light->direction();
    }
}

QDirectionalLight::QDirectionalLight(QDirectionalLightPrivate &dd, QNode *parent)
    : QAbstractLight(dd, parent)
{
}

QDirectionalLight *QDirectionalLight::doClone(bool isClone) const
{
    QDirectionalLight *clone = new QDirectionalLight();
    clone->copy(this);
    clone->d_func()->m_isClone = isClone;
    return clone;
}

void QDirectionalLight::setDirection(const QVector3D &direction)
{
    Q_D(QDirectionalLight);
    if (direction != d->m_direction) {
        d->m_direction = direction;
        emit directionChanged();
        QScenePropertyChangePtr change(new QScenePropertyChange(ComponentAdded, this));
        change->setPropertyName(QByteArrayLiteral("direction"));
        change->setValue(d->m_direction);
        notifyObservers(change);
    }
}

QVector3D QDirectionalLight::direction() const
{
    Q_D(const QDirectionalLight);
    return d->m_direction;
}

QHash<QString, QVariant> QDirectionalLight::lightProperties() const
{
    Q_D(const QDirectionalLight);
    QHash<QString, QVariant> props;
    props[QStringLiteral("direction")] = d->m_direction;
    return props;
}

} // Qt3D

QT_END_NAMESPACE
