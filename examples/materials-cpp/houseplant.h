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

#ifndef HOUSEPLANT_H
#define HOUSEPLANT_H

#include "renderableentity.h"
#include <QEntity>
#include <QNormalDiffuseMapAlphaMaterial>
#include <QNormalDiffuseMapMaterial>

class HousePlant : public Qt3D::QEntity
{
    Q_OBJECT
public:
    explicit HousePlant(Qt3D::QNode *parent = 0);
    ~HousePlant();

    enum PotShape {
        Cross = 0,
        Square,
        Triangle,
        Sphere
    };

    enum Plant {
        Bamboo = 0,
        Palm,
        Pine,
        Spikes,
        Shrub
    };

    void setPotShape(PotShape shape);
    void setPlantType(Plant plant);

    PotShape potShape() const;
    Plant plantType() const;

    void setX(float x);
    void setY(float y);
    void setZ(float z);
    void setScale(float scale);

    float x() const;
    float y() const;
    float z() const;
    float scale() const;

private:
    RenderableEntity *m_pot;
    RenderableEntity *m_plant;
    RenderableEntity *m_cover;

    Qt3D::QNormalDiffuseMapMaterial *m_potMaterial;
    Qt3D::QNormalDiffuseMapAlphaMaterial *m_plantMaterial;
    Qt3D::QNormalDiffuseMapMaterial *m_coverMaterial;

    Plant m_plantType;
    PotShape m_potShape;

    void updatePotShape();
    void updatePlantType();
};

#endif // HOUSEPLANT_H
