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

#ifndef QT3DRENDER_QLIGHT_H
#define QT3DRENDER_QLIGHT_H

#include <Qt3DRender/qt3drender_global.h>
#include <Qt3DRender/qshaderdata.h>

#include <QVector3D>
#include <QColor>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

class QLightPrivate;

class QT3DRENDERSHARED_EXPORT QLight : public QShaderData
{
    Q_OBJECT
    Q_PROPERTY(Type type READ type WRITE setType NOTIFY typeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(float intensity READ intensity WRITE setIntensity NOTIFY intensityChanged)

public :
    enum Type {
        PointLight = 0,
        DirectionalLight,
        SpotLight
    };
    Q_ENUM(Type)

    explicit QLight(Qt3DCore::QNode *parent = 0);
    ~QLight();

    Type type() const;
    QColor color() const;
    float intensity() const;

public Q_SLOTS:
    void setType(Type type);
    void setColor(const QColor &color);
    void setIntensity(float intensity);

protected :
    QLight(QLightPrivate &dd, Qt3DCore::QNode *parent = 0);
    void copy(const Qt3DCore::QNode *ref) Q_DECL_OVERRIDE;

Q_SIGNALS:
    void typeChanged(Type type);
    void colorChanged(const QColor &color);
    void intensityChanged(float intensity);

private:
    Q_DECLARE_PRIVATE(QLight)
    QT3D_CLONEABLE(QLight)
};

} // namespace Qt3DRender

QT_END_NAMESPACE

#endif // QT3DRENDER_QLIGHT_H
