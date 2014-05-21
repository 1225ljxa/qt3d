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
#include "component.h"
#include "qjobmanagerinterface.h"

#include <QDebug>
#include <QMutexLocker>
#include <QReadLocker>
#include <QThread>
#include <QWriteLocker>

QT_BEGIN_NAMESPACE

namespace Qt3D {

QObservable::QObservable()
    : m_observers()
    , m_lock(QReadWriteLock::NonRecursive)
{
}

void QObservable::registerObserver(QObserverInterface *observer)
{
    QWriteLocker locker(&m_lock);
    if (!m_observers.contains(observer))
        m_observers.append(observer);
}

void QObservable::unregisterObserver(QObserverInterface *observer)
{
    QWriteLocker locker(&m_lock);
    m_observers.removeOne(observer);
}

void QObservable::notifyObservers(const QSceneChangePtr &e)
{
    QReadLocker locker(&m_lock);
    Q_FOREACH (QObserverInterface *observer, m_observers)
        observer->sceneChangeEvent(e);
}


QChangeArbiter::QChangeArbiter(QObject *parent)
    : QObject(parent)
    , m_mutex(QMutex::Recursive)
    , m_jobManager(0)
{
    // The QMutex has to be recursive to handle the case where :
    // 1) SyncChanges is called, mutex is locked
    // 2) Changes are distributed
    // 3) An observer decides to register a new observable upon receiving notification
    // 4) registerObserver locks the mutex once again -> we need recursion otherwise deadlock
    // 5) Mutex is unlocked - leaving registerObserver
    // 6) Mutex is unlocked - leaving SyncChanges
}

void QChangeArbiter::initialize(QJobManagerInterface *jobManager)
{
    Q_CHECK_PTR(jobManager);
    m_jobManager = jobManager;

    // Init TLS for the change queues
    m_jobManager->waitForPerThreadFunction(QChangeArbiter::createThreadLocalChangeQueue, this);
}

void QChangeArbiter::distributeQueueChanges(ChangeQueue *changeQueue)
{
    Q_FOREACH (const QSceneChangePtr &change, *changeQueue) {
        // Lookup which observers care about the subject this change came from
        // and distribute the change to them
        switch (change->m_subjectType) {
        case QSceneChange::ObservableType: {
            QObservableInterface *subject = change->m_subject.m_observable;
            if (m_observations.contains(subject)) {
                QObserverList &observers = m_observations[subject];
                Q_FOREACH (const QObserverPair &observer, observers) {
                    if ((change->m_type & observer.first))
                        observer.second->sceneChangeEvent(change);
                }
            }
            break;
        }

        case QSceneChange::ComponentType: {
            Component *subject = change->m_subject.m_component;
            if (m_componentObservations.contains(subject)) {
                QObserverList &observers = m_componentObservations[subject];
                Q_FOREACH (const QObserverPair&observer, observers) {
                    if ((change->m_type & observer.first))
                        observer.second->sceneChangeEvent(change);
                }
            }
            break;
        }
        }
    }
}

void QChangeArbiter::syncChanges()
{
    QMutexLocker locker(&m_mutex);
    Q_FOREACH (QChangeArbiter::ChangeQueue *changeQueue, m_changeQueues) {
        distributeQueueChanges(changeQueue);
        changeQueue->clear();
    }

    Q_FOREACH (ChangeQueue *changeQueue, m_lockingChangeQueues) {
        distributeQueueChanges(changeQueue);
        changeQueue->clear();
    }
}

void QChangeArbiter::registerObserver(QObserverInterface *observer,
                                      QObservableInterface *subject,
                                      ChangeFlags changeFlags)
{
    qDebug() << Q_FUNC_INFO;
    if (!observer || !subject)
        return;

    // Store info about which observers are watching which observables.
    // Protect access as this could be called from any thread
    QMutexLocker locker(&m_mutex);
    QObserverList &observers = m_observations[subject];
    observers.append(QObserverPair(changeFlags, observer));

    // Register ourselves with the observable as the intermediary
    subject->registerObserver(this);
}

void QChangeArbiter::registerObserver(QObserverInterface *observer,
                                      Component *component,
                                      ChangeFlags changeFlags)
{
    qDebug() << Q_FUNC_INFO;
    if (!observer || !component)
        return;

    // Store info about which observers are watching which observables.
    // Protect access as this could be called from any thread
    QMutexLocker locker(&m_mutex);
    QObserverList &observers = m_componentObservations[component];
    observers.append(QObserverPair(changeFlags, observer));

    // Register ourselves with the observable as the intermediary
    component->registerChangeArbiter(this);
}

void QChangeArbiter::unregisterObserver(QObserverInterface *observer,
                                        QObservableInterface *subject)
{
    QMutexLocker locker(&m_mutex);
    if (m_observations.contains(subject)) {
        QObserverList &observers = m_observations[subject];
        for (int i = observers.count() - 1; i >= 0; i--) {
            if (observers[i].second == observer)
                observers.removeAt(i);
        }
    }
}

void QChangeArbiter::unregisterObserver(QObserverInterface *observer, Component *subject)
{
    QMutexLocker locker(&m_mutex);
    if (m_componentObservations.contains(subject)) {
        QObserverList &observers = m_componentObservations[subject];
        for (int i = observers.count() - 1; i >= 0; i--) {
            if (observers[i].second == observer)
                observers.removeAt(i);
        }
    }
}

void QChangeArbiter::sceneChangeEvent(const QSceneChangePtr &e)
{
    //    qDebug() << Q_FUNC_INFO << QThread::currentThread();

    // Add the change to the thread local storage queue - no locking required => yay!
    ChangeQueue *localChangeQueue = m_tlsChangeQueue.localData();
    localChangeQueue->append(e);

    //    qDebug() << "Change queue for thread" << QThread::currentThread() << "now contains" << localChangeQueue->count() << "items";
}

void QChangeArbiter::sceneChangeEventWithLock(const QSceneChangePtr &e)
{
    QMutexLocker locker(&m_mutex);
    sceneChangeEvent(e);
}

void QChangeArbiter::createUnmanagedThreadLocalChangeQueue(void *changeArbiter)
{
    Q_CHECK_PTR(changeArbiter);

    QChangeArbiter *arbiter = static_cast<QChangeArbiter *>(changeArbiter);

    qDebug() << Q_FUNC_INFO << QThread::currentThread();
    if (!arbiter->m_tlsChangeQueue.hasLocalData()) {
        ChangeQueue *localChangeQueue = new ChangeQueue;
        arbiter->m_tlsChangeQueue.setLocalData(localChangeQueue);

        QMutexLocker locker(&(arbiter->m_mutex));
        arbiter->m_lockingChangeQueues.append(localChangeQueue);
    }
}

static void destroyUnmanagedThreadLocalChangeQueue(void *changeArbiter)
{
    // TODO: Implement me!
    Q_UNUSED(changeArbiter);
}

void QChangeArbiter::createThreadLocalChangeQueue(void *changeArbiter)
{
    Q_CHECK_PTR(changeArbiter);

    QChangeArbiter *arbiter = static_cast<QChangeArbiter *>(changeArbiter);

    qDebug() << Q_FUNC_INFO << QThread::currentThread();
    if (!arbiter->m_tlsChangeQueue.hasLocalData()) {
        ChangeQueue *localChangeQueue = new ChangeQueue;
        arbiter->m_tlsChangeQueue.setLocalData(localChangeQueue);

        QMutexLocker locker(&(arbiter->m_mutex));
        arbiter->m_changeQueues.append(localChangeQueue);
    }
}

static void destroyThreadLocalChangeQueue(void *changeArbiter)
{
    // TODO: Implement me!
    Q_UNUSED(changeArbiter);
}

} // namespace Qt3D

QT_END_NAMESPACE
