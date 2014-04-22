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

#include "renderthread.h"
#include "renderer.h"
#include "renderview.h"

#include "renderlogging.h"
#include <QEventLoop>
#include <QTime>
#include <QMutexLocker>

QT_BEGIN_NAMESPACE

namespace Qt3D {

RenderThread::RenderThread( QObject* parent )
    : QThread( parent ),
      m_renderer( 0 ),
      m_eventHandler( 0 )
{
}

void RenderThread::waitForStart( Priority priority )
{
    qCDebug(Render::Backend) << "Starting Render thread and then going to sleep until it is ready for us...";
    m_mutex.lock();
    start( priority );
    m_waitCondition.wait( &m_mutex );
    qCDebug(Render::Backend) << "Render thread is now ready & calling thread is now awake again";
}

void RenderThread::run()
{
    m_mutex.lock();

// Create worker objects
    m_renderer = new Render::Renderer;
    //m_eventHandler = new RenderEventProcessor;

    m_waitCondition.wakeOne();
    m_mutex.unlock();

    // Enter the rendering loop
    while (true) {
        m_renderer->render();
    }
    delete m_renderer;
}

}

QT_END_NAMESPACE
