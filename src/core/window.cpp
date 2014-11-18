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

#include "window.h"

#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QGuiApplication>

#include "qnode.h"
#include "qcamera.h"
#include "qentity.h"
#include "qaspectengine.h"
#include "cameracontroller.h"
#include <Qt3DCore/private/corelogging_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

Window::Window(QScreen *screen)
    : QWindow(screen)
    , m_camera(Q_NULLPTR)
    , m_controller(new CameraController())

{
    setSurfaceType(QSurface::OpenGLSurface);

    resize(1024, 768);

    QSurfaceFormat format;
    format.setVersion(4, 3);
    format.setDepthBufferSize( 24 );
    format.setSamples( 4 );
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);
    create();

    m_controller = new CameraController();

    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(16);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(onUpdate()));
}

Window::~Window()
{
}

void Window::onUpdate()
{
    m_controller->update(1.0 / 60.0);
}

void Window::resizeEvent( QResizeEvent* e )
{
    Q_UNUSED( e );
}

void Window::setCamera(QCamera *camera)
{
    m_camera = camera;
    if (m_camera) {
        qCDebug(Nodes) << "found a camera in the scene";
        m_controller->setCamera(m_camera);
        m_updateTimer->start();
    }
}

void Window::keyPressEvent( QKeyEvent* e )
{
    if (m_controller->keyPressEvent(e))
        return;

    switch ( e->key() )
    {
        case Qt::Key_Escape:
            QGuiApplication::quit();
            break;

        default:
            QWindow::keyPressEvent( e );
    }
}

void Window::keyReleaseEvent( QKeyEvent* e )
{
    if (m_controller->keyReleaseEvent(e))
        return;

    QWindow::keyReleaseEvent(e);
}

void Window::mousePressEvent( QMouseEvent* e )
{
    m_controller->mousePressEvent(e);
}

void Window::mouseReleaseEvent( QMouseEvent* e )
{
    m_controller->mouseReleaseEvent(e);
}

void Window::mouseMoveEvent( QMouseEvent* e )
{
    m_controller->mouseMoveEvent(e);
}

} // namespace Qt3D

QT_END_NAMESPACE
