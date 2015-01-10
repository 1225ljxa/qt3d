/****************************************************************************
**
** Copyright (C) 2015 Klaralvdalens Datakonsult AB (KDAB).
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

#include "boxentity.h"

#include <qmath.h>

BoxEntity::BoxEntity(QNode *parent)
    : Qt3D::QEntity(parent)
    , m_transform(new Qt3D::QTransform())
    , m_translate(new Qt3D::QTranslateTransform())
    , m_mesh(new Qt3D::QCuboidMesh())
    , m_material(new Qt3D::QPhongMaterial())
    , m_angle(0.0f)
    , m_radius(1.0f)
{
    m_transform->addTransform(m_translate);

    connect(m_material, SIGNAL(diffuseChanged()), this, SIGNAL(diffuseColorChanged()));
    m_material->setAmbient(Qt::gray);
    m_material->setSpecular(Qt::white);
    m_material->setShininess(150.0f);

    addComponent(m_transform);
    addComponent(m_mesh);
    addComponent(m_material);
}

void BoxEntity::setDiffuseColor(const QColor &diffuseColor)
{
    m_material->setDiffuse(diffuseColor);
}

void BoxEntity::setAngle(float arg)
{
    if (m_angle == arg)
        return;

    m_angle = arg;
    emit angleChanged();
    updateTransformation();
}

void BoxEntity::setRadius(float arg)
{
    if (m_radius == arg)
        return;

    m_radius = arg;
    emit radiusChanged();
    updateTransformation();
}

QColor BoxEntity::diffuseColor()
{
    return m_material->diffuse();
}

float BoxEntity::angle() const
{
    return m_angle;
}

float BoxEntity::radius() const
{
    return m_radius;
}

void BoxEntity::updateTransformation()
{
    m_translate->setTranslation(QVector3D(cosf(m_angle) * m_radius,
                                          1.0f,
                                          sinf(m_angle) * m_radius));
}

