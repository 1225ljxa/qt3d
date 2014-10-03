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

#include "qabstractmesh.h"
#include "qabstractmesh_p.h"
#include <Qt3DCore/qscenepropertychange.h>

/*!
 * \class QAbstractMesh
 * \namespace Qt3D
 *
 * \brief Provides an abstract class that should be the base of all Mesh
 * primitives in a scene
 *
 * QAbstractMesh subclasses should encapsulate vertices needed to render a Mesh.
 * These should match and be packed according to what the aspect they live in expects.
 *
 * \sa QAbstractTechnique, Component
 */

QT_BEGIN_NAMESPACE

namespace Qt3D {

QAbstractMeshPrivate::QAbstractMeshPrivate(QAbstractMesh *qq)
    : QComponentPrivate(qq)
    , m_dirty(false)
{
}

void QAbstractMeshPrivate::copy(const QNodePrivate *ref)
{
    QNodePrivate::copy(ref);
    const QAbstractMeshPrivate *abstractMesh = static_cast<const QAbstractMeshPrivate *>(ref);
    m_uuid = abstractMesh->m_uuid;
    m_dirty = abstractMesh->m_dirty;
}


QAbstractMesh::QAbstractMesh(QNode *parent)
    : QComponent(*new QAbstractMeshPrivate(this), parent)
{
}

QAbstractMesh::QAbstractMesh(QAbstractMeshPrivate &dd, QNode *parent)
    : QComponent(dd, parent)
{
}

bool QAbstractMesh::isDirty() const
{
    Q_D(const QAbstractMesh);
    return d->m_dirty;
}

void QAbstractMesh::setDirty(bool dirty)
{
    Q_D(QAbstractMesh);
    if (dirty != d->m_dirty) {
        d->m_dirty = dirty;
        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr change(new QScenePropertyChange(ComponentUpdated, this));
            change->setPropertyName(QByteArrayLiteral("meshFunctor"));
            change->setValue(QVariant::fromValue(meshFunctor()));
            d->notifyObservers(change);
            // TO DO see if we can clear the d->m_dirty on request.
            // This would allow to send a single notification for classes that have several property changes occur
            // over a single given frame or maybe that's the job of the QChangeArbiter
            d->m_dirty = false;
        }
    }
}

} // Qt3D

QT_END_NAMESPACE
