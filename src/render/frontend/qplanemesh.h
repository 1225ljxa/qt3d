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

#ifndef QT3D_QPLANEMESH_H
#define QT3D_QPLANEMESH_H

#include <Qt3DRenderer/qt3drenderer_global.h>
#include <Qt3DRenderer/qabstractshapemesh.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

class QPlaneMeshPrivate;

class QT3DRENDERERSHARED_EXPORT QPlaneMesh : public Qt3D::QAbstractShapeMesh
{
    Q_OBJECT

    Q_PROPERTY(float width READ width WRITE setWidth NOTIFY widthChanged)
    Q_PROPERTY(float height READ height WRITE setHeight NOTIFY heightChanged)
    Q_PROPERTY(QSize meshResolution READ meshResolution WRITE setMeshResolution NOTIFY meshResolutionChanged)

public:
    explicit QPlaneMesh(QNode *parent = 0);

    void setWidth(float width);
    float width() const;

    void setHeight(float height);
    float height() const;

    void setMeshResolution(const QSize &resolution);
    QSize meshResolution() const;

    QAbstractMeshFunctorPtr meshFunctor() const Q_DECL_OVERRIDE;

Q_SIGNALS:
    void widthChanged();
    void heightChanged();
    void meshResolutionChanged();

protected:
    QPlaneMesh(QPlaneMeshPrivate &dd, QNode *parent = 0);

private:
    Q_DECLARE_PRIVATE(QPlaneMesh)
    QPlaneMesh *doClone() const Q_DECL_OVERRIDE;
};

} // namespace Qt3D

QT_END_NAMESPACE

#endif // QT3D_QPLANEMESH_H
