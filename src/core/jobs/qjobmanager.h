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

#ifndef QT3D_QJOBMANAGER_H
#define QT3D_QJOBMANAGER_H

#include <Qt3DCore/qjobmanagerinterface.h>
#include <Qt3DCore/qt3dcore_global.h>

#include <Qt3DCore/qjob.h>

#include <job.h>

#include <QVector>

namespace ThreadWeaver {
class Queue;
}

QT_BEGIN_NAMESPACE

namespace Qt3D {

class QJobManager : public QJobManagerInterface
{
    Q_OBJECT
public:
    explicit QJobManager(QObject *parent = 0);

    void initialize() Q_DECL_OVERRIDE;

    void enqueueJobs(const QVector<QJobPtr> &jobQueue) Q_DECL_OVERRIDE;

    void waitForAllJobs() Q_DECL_OVERRIDE;

    void waitForPerThreadFunction(JobFunction func, void *arg) Q_DECL_OVERRIDE;

protected:
    class SynchronizedJob : public ThreadWeaver::Job
    {
    public:
        SynchronizedJob(JobFunction func, void *arg, QAtomicInt *atomicCount)
            : m_func(func)
            , m_arg(arg)
            , m_atomicCount(atomicCount)
        {}

    protected:
        void run(ThreadWeaver::JobPointer self, ThreadWeaver::Thread *thread) Q_DECL_OVERRIDE;

    private:
        JobFunction m_func;
        void *m_arg;
        QAtomicInt *m_atomicCount;
    };

    typedef QSharedPointer<SynchronizedJob> SynchronizedJobPtr;

private:
    ThreadWeaver::Queue *m_weaver;
};

} // namespace Qt3D

QT_END_NAMESPACE

#endif // QT3D_QJOBMANAGER_H
