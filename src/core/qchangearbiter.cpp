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

#include "qchangearbiter.h"
#include "qcomponent.h"
#include "qjobmanagerinterface.h"

#include "corelogging.h"
#include "qsceneobserverinterface.h"
#include <QMutexLocker>
#include <QReadLocker>
#include <QThread>
#include <QWriteLocker>
#include <private/qchangearbiter_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {


QChangeArbiterPrivate::QChangeArbiterPrivate(QChangeArbiter *qq)
    : QObjectPrivate()
    , m_mutex(QMutex::Recursive)
    , m_jobManager(Q_NULLPTR)
{
    q_ptr = qq;
}

QChangeArbiter::QChangeArbiter(QObject *parent)
    : QObject(*new QChangeArbiterPrivate(this), parent)
{
    // The QMutex has to be recursive to handle the case where :
    // 1) SyncChanges is called, mutex is locked
    // 2) Changes are distributed
    // 3) An observer decides to register a new observable upon receiving notification
    // 4) registerObserver locks the mutex once again -> we need recursion otherwise deadlock
    // 5) Mutex is unlocked - leaving registerObserver
    // 6) Mutex is unlocked - leaving SyncChanges
}

QChangeArbiter::~QChangeArbiter()
{
    Q_D(QChangeArbiter);
    if (d->m_jobManager != Q_NULLPTR)
        d->m_jobManager->waitForPerThreadFunction(QChangeArbiter::destroyThreadLocalChangeQueue, this);
    d->m_lockingChangeQueues.clear();
    d->m_changeQueues.clear();
}


QChangeArbiter::QChangeArbiter(QChangeArbiterPrivate &dd, QObject *parent)
    : QObject(dd, parent)
{
}

void QChangeArbiter::initialize(QJobManagerInterface *jobManager)
{
    Q_CHECK_PTR(jobManager);
    Q_D(QChangeArbiter);
    d->m_jobManager = jobManager;

    // Init TLS for the change queues
    d->m_jobManager->waitForPerThreadFunction(QChangeArbiter::createThreadLocalChangeQueue, this);
}

void QChangeArbiter::distributeQueueChanges(ChangeQueue *changeQueue)
{
    Q_D(QChangeArbiter);
    for (int i = 0; i < changeQueue->size(); i++) {
        QSceneChangePtr change = changeQueue->takeFirst();
        // Lookup which observers care about the subject this change came from
        // and distribute the change to them
        if (change.isNull())
            continue;

        switch (change->type()) {

        case NodeCreated: {
            Q_FOREACH (QSceneObserverInterface *observer, d->m_sceneObservers)
                observer->sceneNodeAdded(change);
            break;
        }

        case NodeDeleted:
        case NodeAboutToBeDeleted: {
            Q_FOREACH (QSceneObserverInterface *observer, d->m_sceneObservers)
                observer->sceneNodeRemoved(change);
            break;
        }

        default : {
            switch (change->observableType()) {

            case QSceneChange::Observable: {
                QObservableInterface *subject = change->subject().m_observable;
                if (d->m_aspectObservations.contains(subject)) {
                    QObserverList &observers = d->m_aspectObservations[subject];
                    Q_FOREACH (const QObserverPair &observer, observers) {
                        if ((change->type() & observer.first))
                            observer.second->sceneChangeEvent(change);
                    }
                }
                break;
            }

            case QSceneChange::Node: {
                QNode *subject = change->subject().m_node;
                if (d->m_nodeObservations.contains(subject->uuid())) {
                    QObserverList &observers = d->m_nodeObservations[subject->uuid()];
                    Q_FOREACH (const QObserverPair&observer, observers) {
                        if ((change->type() & observer.first))
                            observer.second->sceneChangeEvent(change);
                    }
                }
                break;
            }

            } // observableType switch
            break;
        } // default

        } // change type switch
    }
}

QThreadStorage<QChangeArbiter::ChangeQueue *> *QChangeArbiter::tlsChangeQueue()
{
    Q_D(QChangeArbiter);
    return &(d->m_tlsChangeQueue);
}

void QChangeArbiter::appendChangeQueue(QChangeArbiter::ChangeQueue *queue)
{
    Q_D(QChangeArbiter);
    QMutexLocker locker(&(d->m_mutex));
    d->m_changeQueues.append(queue);
}

void QChangeArbiter::appendLockingChangeQueue(QChangeArbiter::ChangeQueue *queue)
{
    Q_D(QChangeArbiter);
    QMutexLocker locker(&(d->m_mutex));
    d->m_lockingChangeQueues.append(queue);
}

void QChangeArbiter::syncChanges()
{
    Q_D(QChangeArbiter);
    QMutexLocker locker(&d->m_mutex);
    Q_FOREACH (QChangeArbiter::ChangeQueue *changeQueue, d->m_changeQueues) {
        distributeQueueChanges(changeQueue);
    }

    Q_FOREACH (ChangeQueue *changeQueue, d->m_lockingChangeQueues) {
        distributeQueueChanges(changeQueue);
    }
}

void QChangeArbiter::registerObserver(QObserverInterface *observer,
                                      const QUuid &nodeId,
                                      ChangeFlags changeFlags)
{
    Q_D(QChangeArbiter);
    QMutexLocker locker(&d->m_mutex);
    QObserverList &observerList = d->m_nodeObservations[nodeId];
    observerList.append(QObserverPair(changeFlags, observer));
}

void QChangeArbiter::registerObserver(QObserverInterface *observer,
                                      QObservableInterface *observable,
                                      ChangeFlags changeFlags)
{
    Q_D(QChangeArbiter);
    QMutexLocker locker(&d->m_mutex);
    QObserverList &observerList = d->m_aspectObservations[observable];
    registerObserverHelper(observerList, observer, observable, changeFlags);
}

void QChangeArbiter::registerObserver(QObserverInterface *observer,
                                      QNode *observable,
                                      ChangeFlags changeFlags)
{
    Q_D(QChangeArbiter);
    QMutexLocker locker(&d->m_mutex);
    if (!observable)
        return;
    QObserverList &observerList = d->m_nodeObservations[observable->uuid()];
    registerObserverHelper(observerList, observer, observable, changeFlags);
}

// Called from the QAspectThread context, no need to lock
void QChangeArbiter::registerSceneObserver(QSceneObserverInterface *observer)
{
    if (observer == Q_NULLPTR) {
        qCWarning(ChangeArbiter) << Q_FUNC_INFO << "SceneObserverInterface is null";
        return ;
    }
    Q_D(QChangeArbiter);
    if (!d->m_sceneObservers.contains(observer))
        d->m_sceneObservers << observer;
}

void QChangeArbiter::registerObserverHelper(QObserverList &observerList,
                                            QObserverInterface *observer,
                                            QObservableInterface *observable,
                                            ChangeFlags changeFlags)
{
    Q_ASSERT(observer);
    Q_ASSERT(observable);
    qCDebug(ChangeArbiter) << Q_FUNC_INFO << observable;

    // Store info about which observers are watching which observables.
    // Protect access as this could be called from any thread
    observerList.append(QObserverPair(changeFlags, observer));

    // Register ourselves with the observable as the intermediary
    observable->registerObserver(this);
}

void QChangeArbiter::unregisterObserver(QObserverInterface *observer,
                                        QObservableInterface *subject)
{
    Q_D(QChangeArbiter);
    QMutexLocker locker(&d->m_mutex);
    if (d->m_aspectObservations.contains(subject)) {
        QObserverList &observers = d->m_aspectObservations[subject];
        for (int i = observers.count() - 1; i >= 0; i--) {
            if (observers[i].second == observer)
                observers.removeAt(i);
        }
    }
}

void QChangeArbiter::unregisterObserver(QObserverInterface *observer, QNode *subject)
{
    unregisterObserver(observer, subject->uuid());
}

void QChangeArbiter::unregisterObserver(QObserverInterface *observer, const QUuid &nodeId)
{
    Q_D(QChangeArbiter);
    QMutexLocker locker(&d->m_mutex);
    if (d->m_nodeObservations.contains(nodeId)) {
        QObserverList &observers = d->m_nodeObservations[nodeId];
        for (int i = observers.count() - 1; i >= 0; i--) {
            if (observers[i].second == observer)
                observers.removeAt(i);
        }
    }
}

// Called from the QAspectThread context, no need to lock
void QChangeArbiter::unregisterSceneObserver(QSceneObserverInterface *observer)
{
    Q_D(QChangeArbiter);
    if (observer != Q_NULLPTR)
        d->m_sceneObservers.removeOne(observer);
}

void QChangeArbiter::sceneChangeEvent(const QSceneChangePtr &e)
{
    //    qCDebug(ChangeArbiter) << Q_FUNC_INFO << QThread::currentThread();

    Q_D(QChangeArbiter);
    // Add the change to the thread local storage queue - no locking required => yay!
    ChangeQueue *localChangeQueue = d->m_tlsChangeQueue.localData();
    localChangeQueue->append(e);

    //    qCDebug(ChangeArbiter) << "Change queue for thread" << QThread::currentThread() << "now contains" << localChangeQueue->count() << "items";
}

void QChangeArbiter::sceneChangeEventWithLock(const QSceneChangePtr &e)
{
    Q_D(QChangeArbiter);
    QMutexLocker locker(&d->m_mutex);
    sceneChangeEvent(e);
}

void QChangeArbiter::createUnmanagedThreadLocalChangeQueue(void *changeArbiter)
{
    Q_CHECK_PTR(changeArbiter);

    QChangeArbiter *arbiter = static_cast<QChangeArbiter *>(changeArbiter);

    qCDebug(ChangeArbiter) << Q_FUNC_INFO << QThread::currentThread();
    if (!arbiter->tlsChangeQueue()->hasLocalData()) {
        ChangeQueue *localChangeQueue = new ChangeQueue;
        arbiter->tlsChangeQueue()->setLocalData(localChangeQueue);
        arbiter->appendLockingChangeQueue(localChangeQueue);
    }
}

void QChangeArbiter::destroyUnmanagedThreadLocalChangeQueue(void *changeArbiter)
{
    // TODO: Implement me!
    Q_UNUSED(changeArbiter);
}

void QChangeArbiter::createThreadLocalChangeQueue(void *changeArbiter)
{
    Q_CHECK_PTR(changeArbiter);

    QChangeArbiter *arbiter = static_cast<QChangeArbiter *>(changeArbiter);

    qCDebug(ChangeArbiter) << Q_FUNC_INFO << QThread::currentThread();
    if (!arbiter->tlsChangeQueue()->hasLocalData()) {
        ChangeQueue *localChangeQueue = new ChangeQueue;
        arbiter->tlsChangeQueue()->setLocalData(localChangeQueue);
        arbiter->appendChangeQueue(localChangeQueue);
    }
}

void QChangeArbiter::destroyThreadLocalChangeQueue(void *changeArbiter)
{
    // TODO: Implement me!
    Q_UNUSED(changeArbiter);
}

} // namespace Qt3D

QT_END_NAMESPACE
