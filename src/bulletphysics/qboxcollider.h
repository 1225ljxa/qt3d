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

#ifndef QT3D_BULLETPHYSICS_QBOXCOLLIDER_H
#define QT3D_BULLETPHYSICS_QBOXCOLLIDER_H

#include <Qt3DBulletPhysics/qt3dbulletphysics_global.h>
#include <Qt3DBulletPhysics/qabstractcollider.h>

#include <QtGui/qvector3d.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {
namespace BulletPhysics {

class QBoxColliderPrivate;

class QBoxCollider : public Qt3D::BulletPhysics::QAbstractCollider
{
    Q_OBJECT

    Q_PROPERTY(QVector3D center READ center WRITE setCenter NOTIFY centerChanged)
    Q_PROPERTY(float centerX READ centerX WRITE setCenterX NOTIFY centerXChanged)
    Q_PROPERTY(float centerY READ centerY WRITE setCenterY NOTIFY centerYChanged)
    Q_PROPERTY(float centerZ READ centerZ WRITE setCenterZ NOTIFY centerZChanged)
    Q_PROPERTY(QVector3D halfExtents READ halfExtents WRITE setHalfExtents NOTIFY halfExtentsChanged)

public:
    explicit QBoxCollider(Qt3D::QNode *parent = 0);

    void setCenter(const QVector3D &center);
    QVector3D center() const;

    void setHalfExtents(const QVector3D &halfExtents);
    QVector3D halfExtents() const;

    void setCenterX(float x);
    float centerX() const;

    void setCenterY(float y);
    float centerY() const;

    void setCenterZ(float z);
    float centerZ() const;

Q_SIGNALS:
    void centerXChanged();
    void centerYChanged();
    void centerZChanged();
    void centerChanged();
    void halfExtentsChanged();

protected:
    QBoxCollider(QBoxColliderPrivate &dd, QNode *parent = 0);
    void copy(const QNode *ref) Q_DECL_OVERRIDE;

private:
    Q_DECLARE_PRIVATE(QBoxCollider)
    QT3D_CLONEABLE(QBoxCollider)
};

} // namespace BulletPhysics
} // namespace Qt3D

QT_END_NAMESPACE

#endif // QT3D_BULLETPHYSICS_QBOXCOLLIDER_H
