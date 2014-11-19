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

#include "qscene.h"
#include <QHash>
#include <QReadLocker>
#include <Qt3DCore/qnode.h>
#include <Qt3DCore/private/qchangearbiter_p.h>
#include <Qt3DCore/private/qobservableinterface_p.h>
#include <Qt3DCore/private/qnode_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

class QScenePrivate
{
public:
    QScenePrivate(QScene *qq)
        : q_ptr(qq)
        , m_arbiter(Q_NULLPTR)
    {
    }

    Q_DECLARE_PUBLIC(QScene)
    QScene *q_ptr;
    QHash<QNodeId, QNode *> m_nodeLookupTable;
    QMultiHash<QNodeId, QNodeId> m_componentToEntities;
    QMultiHash<QNodeId, QObservableInterface *> m_observablesLookupTable;
    QHash<QObservableInterface *, QNodeId> m_observableToUuid;
    QChangeArbiter *m_arbiter;
    QHash<QNodeId, QNode *> m_clonesLookupTable;
    mutable QReadWriteLock m_lock;
};


QScene::QScene()
    : d_ptr(new QScenePrivate(this))
{
}

// Called by any thread
void QScene::addObservable(QObservableInterface *observable, const QNodeId &uuid)
{
    Q_D(QScene);
    QWriteLocker lock(&d->m_lock);
    d->m_observablesLookupTable.insert(uuid, observable);
    d->m_observableToUuid.insert(observable, uuid);
    if (d->m_arbiter != Q_NULLPTR)
        observable->setArbiter(d->m_arbiter);
}

// Called by main thread only
void QScene::addObservable(QNode *observable)
{
    Q_D(QScene);
    if (observable != Q_NULLPTR) {
        QWriteLocker lock(&d->m_lock);
        d->m_nodeLookupTable.insert(observable->uuid(), observable);
        if (d->m_arbiter != Q_NULLPTR)
            observable->d_func()->setArbiter(d->m_arbiter);
    }
}

// Called by any thread
void QScene::removeObservable(QObservableInterface *observable, const QNodeId &uuid)
{
    Q_D(QScene);
    QWriteLocker lock(&d->m_lock);
    d->m_observablesLookupTable.remove(uuid, observable);
    d->m_observableToUuid.remove(observable);
    observable->setArbiter(Q_NULLPTR);
}

// Called by main thread
void QScene::removeObservable(QNode *observable)
{
    Q_D(QScene);
    if (observable != Q_NULLPTR) {
        QWriteLocker lock(&d->m_lock);
        QNodeId nodeUuid = observable->uuid();
        QObservableList observables = d->m_observablesLookupTable.values(nodeUuid);
        Q_FOREACH (QObservableInterface *o, observables) {
            o->setArbiter(Q_NULLPTR);
            d->m_observableToUuid.remove(o);
        }
        d->m_observablesLookupTable.remove(nodeUuid);
        d->m_nodeLookupTable.remove(nodeUuid);
        observable->d_func()->setArbiter(Q_NULLPTR);
    }
}

// Called by any thread
QObservableList QScene::lookupObservables(const QNodeId &uuid) const
{
    Q_D(const QScene);
    QReadLocker lock(&d->m_lock);
    return d->m_observablesLookupTable.values(uuid);
}

// Called by any thread
QNode *QScene::lookupNode(const QNodeId &uuid) const
{
    Q_D(const QScene);
    QReadLocker lock(&d->m_lock);
    return d->m_nodeLookupTable.value(uuid);
}

QNodeId QScene::nodeIdFromObservable(QObservableInterface *observable) const
{
    Q_D(const QScene);
    QReadLocker lock(&d->m_lock);
    return d->m_observableToUuid.value(observable);
}

void QScene::setArbiter(QChangeArbiter *arbiter)
{
    Q_D(QScene);
    d->m_arbiter = arbiter;
}

QList<QNodeId> QScene::entitiesForComponent(const QNodeId &uuid) const
{
    Q_D(const QScene);
    QReadLocker lock(&d->m_lock);
    return d->m_componentToEntities.values(uuid);
}

void QScene::addEntityForComponent(const QNodeId &componentUuid, const QNodeId &entityUuid)
{
    Q_D(QScene);
    QWriteLocker lock(&d->m_lock);
    d->m_componentToEntities.insert(componentUuid, entityUuid);
}

void QScene::removeEntityForComponent(const QNodeId &componentUuid, const QNodeId &entityUuid)
{
    Q_D(QScene);
    QWriteLocker lock(&d->m_lock);
    d->m_componentToEntities.remove(componentUuid, entityUuid);
}

} // Qt3D

QT_END_NAMESPACE
