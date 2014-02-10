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

#include "qaspectthread.h"

#include <qaspectmanager.h>

#include <QDebug>
#include <QMutexLocker>

namespace Qt3D {

QAspectThread::QAspectThread(QObject *parent)
    : QThread(parent)
    , m_aspectManager(0)
{
    qDebug() << Q_FUNC_INFO;
}

void QAspectThread::waitForStart(Priority priority)
{
    qDebug() << "Starting QAspectThread and going to sleep until it is ready for us...";
    m_mutex.lock();
    start(priority);
    m_waitCondition.wait(&m_mutex);
    qDebug() << "QAspectThead is now ready & calling thread is now awake again";
}

void QAspectThread::run()
{
    qDebug() << "Entering void QAspectThread::run()";

    // Lock mutex and create worker objects
    QMutexLocker locker(&m_mutex);
    m_aspectManager = new QAspectManager;

    // Wake up the calling thread now that our worker objects are ready for action
    m_waitCondition.wakeOne();
    m_mutex.unlock();

    // Load and initialize the aspects and any other core services
    m_aspectManager->initialize();

    // Enter the main loop
    m_aspectManager->exec();

    // Clean up
    m_aspectManager->shutdown();
    delete m_aspectManager;

    qDebug() << "Exiting void QAspectThread::run()";
}

} // namespace Qt3D
