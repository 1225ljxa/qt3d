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

#ifndef QT3D_QABSTRACTLIGHT_H
#define QT3D_QABSTRACTLIGHT_H

#include <Qt3DCore/qcomponent.h>
#include <Qt3DRenderer/qt3drenderer_global.h>

#include <QVector3D>
#include <QColor>

QT_BEGIN_NAMESPACE

namespace Qt3D
{

class QAbstractLightPrivate;

class QT3DRENDERERSHARED_EXPORT QAbstractLight : public QComponent
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(float intensity READ intensity WRITE setIntensity NOTIFY intensityChanged)

public :

    explicit QAbstractLight(QNode *parent = 0);

    virtual void copy(const QNode *ref) Q_DECL_OVERRIDE;

    QColor color() const;
    void setColor(const QColor &color);

    float intensity() const;
    void setIntensity(float intensity);

    // All properties returned here will be used to set the shader's Light struct
    // members. In addition all Light have an implicit vec3 position attributes that
    // is set at runtime with the light's current world position in the scene.
    virtual QHash<QString, QVariant> lightProperties() const = 0;

    // Returns the name used as the struct name for a given lightType to be used in shaders
    virtual const QString lightBlockName() const = 0;
    virtual const QString lightUniformName() const = 0;

protected :
    Q_DECLARE_PRIVATE(QAbstractLight)
    QAbstractLight(QAbstractLightPrivate &dd, QNode *parent = 0);

Q_SIGNALS:
    void colorChanged();
    void intensityChanged();
};

} // Qt3D

QT_END_NAMESPACE

#endif // QT3D_LIGHT_H
