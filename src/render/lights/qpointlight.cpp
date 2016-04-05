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

#include "qpointlight.h"
#include "qpointlight_p.h"

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

/*
  Expected Shader struct

  \code

  struct PointLight
  {
   vec3 position;
   vec4 color;
   float intensity;
  };

  uniform PointLight pointLights[10];

  \endcode
 */

QPointLightPrivate::QPointLightPrivate(QLight::Type type)
    : QLightPrivate(type)
    , m_constantAttenuation(0.0f)
    , m_linearAttenuation(0.0f)
    , m_quadraticAttenuation(0.002f)
{
}

/*!
  \class Qt3DRender::QPointLight
  \inmodule Qt3DRender
  \since 5.5

 */

/*!
    \qmltype PointLight
    \instantiates Qt3DRender::QPointLight
    \inherits AbstractLight
    \inqmlmodule Qt3D.Render
    \since 5.5
    \brief For OpenGL ...
*/

/*!
  \fn Qt3DRender::QPointLight::QPointLight(Qt3DCore::QNode *parent)
  Constructs a new QPointLight with the specified \a parent.
 */
QPointLight::QPointLight(QNode *parent)
    : QLight(*new QPointLightPrivate, parent)
{
}

/*! \internal */
QPointLight::QPointLight(QPointLightPrivate &dd, QNode *parent)
    : QLight(dd, parent)
{
}

float QPointLight::constantAttenuation() const
{
    Q_D(const QPointLight);
    return d->m_constantAttenuation;
}

void QPointLight::setConstantAttenuation(float value)
{
    Q_D(QPointLight);
    if (d->m_constantAttenuation != value) {
        d->m_constantAttenuation = value;
        emit constantAttenuationChanged(value);
    }
}

float QPointLight::linearAttenuation() const
{
    Q_D(const QPointLight);
    return d->m_linearAttenuation;
}

void QPointLight::setLinearAttenuation(float value)
{
    Q_D(QPointLight);
    if (d->m_linearAttenuation != value) {
        d->m_linearAttenuation = value;
        emit linearAttenuationChanged(value);
    }
}

float QPointLight::quadraticAttenuation() const
{
    Q_D(const QPointLight);
    return d->m_quadraticAttenuation;
}

void QPointLight::setQuadraticAttenuation(float value)
{
    Q_D(QPointLight);
    if (d->m_quadraticAttenuation != value) {
        d->m_quadraticAttenuation = value;
        emit quadraticAttenuationChanged(value);
    }
}

} // namespace Qt3DRender

QT_END_NAMESPACE
