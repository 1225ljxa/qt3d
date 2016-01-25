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

#ifndef QT3DRENDER_QMESH_H
#define QT3DRENDER_QMESH_H

#include <Qt3DRender/qt3drender_global.h>
#include <Qt3DRender/qgeometryrenderer.h>
#include <QUrl>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

class QMeshPrivate;

/**
* @brief Simple static mesh
*
*/
class QT3DRENDERSHARED_EXPORT QMesh : public QGeometryRenderer
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)
    Q_PROPERTY(QString subMesh READ subMesh WRITE setSubMesh NOTIFY subMeshChanged)

public:
    explicit QMesh(Qt3DCore::QNode *parent = 0);
    ~QMesh();

    QUrl source() const;
    QString subMesh() const;

public Q_SLOTS:
    void setSource(const QUrl &source);
    void setSubMesh(const QString &subMesh);

Q_SIGNALS:
    void sourceChanged(const QUrl &source);
    void subMeshChanged(const QString &subMesh);

protected:
    QMesh(QMeshPrivate &dd, Qt3DCore::QNode *parent = 0);

private:
    Q_DECLARE_PRIVATE(QMesh)
};

}

QT_END_NAMESPACE

#endif // of QT3DRENDER_QMESH_H
