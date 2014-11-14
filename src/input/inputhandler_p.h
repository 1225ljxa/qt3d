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

#ifndef QT3D_INPUT_INPUTHANDLER_P_H
#define QT3D_INPUT_INPUTHANDLER_P_H

#include <Qt3DInput/qt3dinput_global.h>
#include <Qt3DInput/private/handle_types_p.h>
#include <QKeyEvent>
#include <QMutex>

QT_BEGIN_NAMESPACE

class QWindow;

namespace Qt3D {

namespace Input {

class KeyboardInputManager;
class KeyboardControllerManager;
class KeyboardEventFilter;

class InputHandler
{
public:
    InputHandler();

    void setWindow(QWindow *window);
    inline QWindow *window() const { return m_window; }

    inline KeyboardControllerManager *keyboardControllerManager() const { return m_keyboardControllerManager; }
    inline KeyboardInputManager *keyboardInputManager() const  { return m_keyboardInputManager; }

    void appendKeyEvent(const QKeyEvent &event);
    QList<QKeyEvent> pendingKeyEvents() const;
    void clearPendingKeyEvents();

    void appendKeyboardController(HKeyboardController controller);
    void removeKeyboardController(HKeyboardController controller);

private:
    KeyboardControllerManager *m_keyboardControllerManager;
    KeyboardInputManager *m_keyboardInputManager;
    QVector<HKeyboardController> m_activeKeyboardControllers;
    QWindow *m_window;
    KeyboardEventFilter *m_keyboardEventFilter;
    QList<QKeyEvent> m_pendingEvents;
    mutable QMutex m_mutex;
};

} // Input

} // Qt3D

QT_END_NAMESPACE

#endif // QT3D_INPUT_INPUTHANDLER_P_H
