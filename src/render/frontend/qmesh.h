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

#ifndef QT3D_QMESH_H
#define QT3D_QMESH_H

#include <Qt3DCore/qabstractmesh.h>
#include <Qt3DRenderer/qt3drenderer_global.h>
#include <Qt3DRenderer/meshdata.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

class QMeshPrivate;

/**
* @brief Simple static mesh
*
*/
class QT3DRENDERERSHARED_EXPORT QMesh : public QAbstractMesh
{
    Q_OBJECT

public:
    explicit QMesh(Node *parent = 0);

    void setSource(const QString &source) Q_DECL_OVERRIDE;

    // Not const because subclasses may want to perform
    // more than just returning a MeshData straight away
    virtual MeshDataPtr data();
    void setData(MeshDataPtr d);

protected:
    Q_DECLARE_PRIVATE(QMesh)
    QMesh(QMeshPrivate &dd, Node *parent = 0);

};

}

QT_END_NAMESPACE

#endif // of QT3D_QMESH_H
