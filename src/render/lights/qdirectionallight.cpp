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

#include "qdirectionallight.h"
#include "qdirectionallight_p.h"
#include <Qt3DCore/qscenepropertychange.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

/*
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

/*!
    \class Qt3DRender::QDirectionalLightPrivate
    \internal
*/
QDirectionalLightPrivate::QDirectionalLightPrivate()
    : QLightPrivate(QLight::DirectionalLight)
    , m_direction(0.0f, -1.0f, 0.0f)
{
}

void QDirectionalLight::copy(const QNode *ref)
{
    const QDirectionalLight *light = static_cast<const QDirectionalLight*>(ref);
    d_func()->m_direction = light->d_func()->m_direction;
    // This needs to be last otherwise, properties value won't be copied
    // as we use shader introspection in QShaderData::copy
    QLight::copy(ref);
}

QDirectionalLight::QDirectionalLight(QNode *parent)
    : QLight(*new QDirectionalLightPrivate, parent)
{
}

/*! \internal */
QDirectionalLight::QDirectionalLight(QDirectionalLightPrivate &dd, QNode *parent)
    : QLight(dd, parent)
{
}

void QDirectionalLight::setDirection(const QVector3D &direction)
{
    Q_D(QDirectionalLight);
    if (direction != d->m_direction) {
        d->m_direction = direction;
        emit directionChanged(direction);
    }
}

QVector3D QDirectionalLight::direction() const
{
    Q_D(const QDirectionalLight);
    return d->m_direction;
}

} // namespace Qt3DRender

QT_END_NAMESPACE
