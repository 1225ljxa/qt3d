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

#ifndef QT3D_QASPECTMANAGER_H
#define QT3D_QASPECTMANAGER_H

#include <QObject>
#include <QList>
#include <Qt3DCore/qt3dcore_global.h>

QT_BEGIN_NAMESPACE

class QWaitCondition;
class QWindow;

namespace Qt3D {

class QNode;
class QAbstractAspect;
class QChangeArbiter;
class QJobManagerInterface;
class QSceneObserverInterface;
class QAspectManagerPrivate;

class QT3DCORESHARED_EXPORT QAspectManager : public QObject
{
    Q_OBJECT
public:
    explicit QAspectManager(QObject *parent = 0);
    ~QAspectManager();

public Q_SLOTS:
    void initialize();
    void shutdown();

    void setRoot(Qt3D::QNode *rootObject);
    void setWindow(QWindow* window);
    void registerAspect(Qt3D::QAbstractAspect *aspect);
    QWindow *window() const;

    void exec();
    void quit();

    const QList<QAbstractAspect *> &aspects() const;
    QJobManagerInterface *jobManager() const;
    QChangeArbiter *changeArbiter() const;

protected:
    Q_DECLARE_PRIVATE(QAspectManager)
    QAspectManager(QAspectManagerPrivate &dd, QObject *parent = 0);
};

} // namespace Qt3D

QT_END_NAMESPACE

#endif // QT3D_QASPECTMANAGER_H
