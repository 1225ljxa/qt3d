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

#include <QDebug>
#include <QQmlComponent>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QGuiApplication>

#include "node.h"
#include "camera.h"
#include "entity.h"
#include <qaspectengine.h>

#include "cameracontroller.h"

QT_BEGIN_NAMESPACE

namespace Qt3D {

Window::Window(QScreen *screen)
    : QWindow(screen)
    , m_engine(new QQmlEngine)
    , m_aspectEngine(new QAspectEngine(this))
    , m_camera(NULL)
    , m_controller(NULL)

{
    setSurfaceType(QSurface::OpenGLSurface);

    resize(1024, 768);

    QSurfaceFormat format;
    format.setVersion(4, 3);
    format.setDepthBufferSize( 24 );
    format.setProfile(QSurfaceFormat::CoreProfile);
    setFormat(format);

    m_aspectEngine->initialize();
    m_aspectEngine->setWindow(this);

    m_controller = new CameraController;

    m_updateTimer = new QTimer(this);
    m_updateTimer->setInterval(16);
    connect(m_updateTimer, SIGNAL(timeout()), this, SLOT(onUpdate()));
}

Window::~Window()
{
    m_aspectEngine->shutdown();
    delete m_aspectEngine;
}

Window::Status Window::status() const
{
    if (!m_engine)
        return Error;

    if (!m_component)
        return Null;

    return Status(m_component->status());
}

/*!
 * Registers an Aspect module to the AspectEngine;
 */
void Window::registerAspect(AbstractAspect *aspect)
{
    m_aspectEngine->registerAspect(aspect);
}

void Window::setSource( const QUrl& source )
{
    if (!m_engine) {
        qWarning() << "Window: invalid qml engine.";
        return;
    }

    if (m_root) {
        m_aspectEngine->shutdown();
        m_aspectEngine->setRoot(0);
        m_root = QSharedPointer<QObject>();
    }

    if (m_component)
        m_component = QSharedPointer<QQmlComponent>();

    if (!source.isEmpty()) {
        m_component = QSharedPointer<QQmlComponent>(new QQmlComponent(m_engine.data(), source, this));
        if (!m_component->isLoading()) {
            continueExecute();
        } else {
            QObject::connect(m_component.data(), SIGNAL(statusChanged(QQmlComponent::Status)),
                             this, SLOT(continueExecute()));
        }
    }
}

void Window::continueExecute()
{
    qDebug() << Q_FUNC_INFO;

    disconnect(m_component.data(), SIGNAL(statusChanged(QQmlComponent::Status)),
               this, SLOT(continueExecute()));

    if (m_component->isError()) {
        QList<QQmlError> errorList = m_component->errors();
        Q_FOREACH ( const QQmlError& error, errorList ) {
            QMessageLogger(error.url().toString().toLatin1().constData(), error.line(), 0).warning()
                << error;
        }
        emit statusChanged(status());
        return;
    }

    QObject* obj = m_component->create();

    if (m_component->isError()) {
        QList<QQmlError> errorList = m_component->errors();
        Q_FOREACH ( const QQmlError& error, errorList ) {
            QMessageLogger(error.url().toString().toLatin1().constData(), error.line(), 0).warning()
                << error;
        }
        emit statusChanged(status());
        return;
    }

    setRootObject(obj);
    emit statusChanged(status());
}

void Window::onUpdate()
{
    m_controller->update(1.0 / 60.0);
}

void Window::setRootObject( QObject* obj )
{
    if (m_root == obj)
        return;

    if (obj) {
        obj->setParent( this );
        m_root = QSharedPointer<QObject>(obj);
    }

    m_aspectEngine->setRoot(obj);

    // Hook up controller input to camera
    // TODO: Do this more generically as we may want keyboard ot control an Entity etc
    // What happens if there is no camera
    // What happens if at some point the camera is added but not directly when the scene is created ?
    // eg scene file provided and camera tree node created after parsing ?
    // What happens if there are multiple cameras in the scene ?
    m_camera = Entity::findEntityInTree<Camera>(qobject_cast<Node *>(m_root.data()));
    if (m_camera) {
        qDebug() << "found a camera in the scene";
        m_controller->setCamera(m_camera);
        m_updateTimer->start();
    }

//
//    Entity *ball = node->findChild<Entity *>(QStringLiteral("ball"));
//    if (ball) {
//        QList<Component *> components = ball->components();
//        qDebug() << components;
//    }

    //Node *node = qobject_cast<Node *>(m_root.data());
    //if (node)
        //node->dump();
}

void Window::resizeEvent( QResizeEvent* e )
{
    Q_UNUSED( e );

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
