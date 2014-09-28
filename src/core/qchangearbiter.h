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

#ifndef QT3D_QCHANGEARBITER_H
#define QT3D_QCHANGEARBITER_H

#include <Qt3DCore/qt3dcore_global.h>
#include <QObject>
#include <Qt3DCore/qobserverinterface.h>
#include <Qt3DCore/qscenechange.h>

#include <QFlags>
#include <QReadWriteLock>
#include <QVariant>
#include <QVector>
#include <QPair>
#include <QThreadStorage>

QT_BEGIN_NAMESPACE

namespace Qt3D {

class QNode;
class QObservableInterface;
class QJobManagerInterface;
class QChangeArbiterPrivate;
class QSceneObserverInterface;
class QPostman;

class QT3DCORESHARED_EXPORT QChangeArbiter : public QObject,
                                             public QObserverInterface
{
    Q_OBJECT
public:
    explicit QChangeArbiter(QObject *parent = 0);
    ~QChangeArbiter();

    void initialize(Qt3D::QJobManagerInterface *jobManager);

    void syncChanges();

    void registerObserver(QObserverInterface *observer,
                          const QUuid &nodeId,
                          ChangeFlags changeFlags = AllChanges);

    void registerObserver(QObserverInterface *observer,
                          QObservableInterface *observable,
                          ChangeFlags changeFlags = AllChanges);

    void registerObserver(QObserverInterface *observer,
                          QNode *observable,
                          ChangeFlags changeFlags = AllChanges);

    void registerSceneObserver(QSceneObserverInterface *observer);

    void unregisterObserver(QObserverInterface *observer,
                            QObservableInterface *subject);

    void unregisterObserver(QObserverInterface *observer,
                            QNode *subject);

    void unregisterObserver(QObserverInterface *observer,
                            const QUuid &nodeId);

    void unregisterSceneObserver(QSceneObserverInterface *observer);

    void sceneChangeEvent(const QSceneChangePtr &e) Q_DECL_OVERRIDE;                 // QObserverInterface impl
    void sceneChangeEventWithLock(const QSceneChangePtr &e);

    Q_INVOKABLE void setPostman(Qt3D::QPostman *postman);

    static void createUnmanagedThreadLocalChangeQueue(void *changeArbiter);
    static void destroyUnmanagedThreadLocalChangeQueue(void *changeArbiter);
    static void createThreadLocalChangeQueue(void *changeArbiter);
    static void destroyThreadLocalChangeQueue(void *changeArbiter);

protected:
    typedef QVector<QSceneChangePtr> ChangeQueue;
    typedef QPair<ChangeFlags, QObserverInterface *> QObserverPair;
    typedef QList<QObserverPair> QObserverList;

    void registerObserverHelper(QObserverList &observerList,
                                QObserverInterface *observer,
                                QObservableInterface *observable,
                                ChangeFlags changeFlags);

    void distributeQueueChanges(ChangeQueue *queue);

    QThreadStorage<ChangeQueue *> *tlsChangeQueue();
    void appendChangeQueue(ChangeQueue *queue);
    void appendLockingChangeQueue(ChangeQueue *queue);

    Q_DECLARE_PRIVATE(QChangeArbiter)
    QChangeArbiter(QChangeArbiterPrivate &dd, QObject *parent = 0);
};

} // namespace Qt3D

QT_END_NAMESPACE

#endif // QT3D_QCHANGEARBITER_H
