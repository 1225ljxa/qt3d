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

#include "assignkeyboardfocusjob_p.h"
#include "keyboardcontroller_p.h"
#include "keyboardinput_p.h"
#include "inputhandler_p.h"
#include "inputmanagers_p.h"

QT_BEGIN_NAMESPACE

namespace Qt3D {

namespace Input {

AssignKeyboardFocusJob::AssignKeyboardFocusJob(const QNodeUuid &controller)
    : QAspectJob()
    , m_inputHandler(Q_NULLPTR)
    , m_keyboardController(controller)
{
}

void AssignKeyboardFocusJob::setInputHandler(InputHandler *handler)
{
    m_inputHandler = handler;
}

void AssignKeyboardFocusJob::run()
{
    KeyboardController *controller = m_inputHandler->keyboardControllerManager()->lookupResource(m_keyboardController);
    Q_FOREACH (const HKeyboardInput handle, controller->keyboardInputsHandles()) {
        KeyboardInput *input = m_inputHandler->keyboardInputManager()->data(handle);
        if (input) {
            bool hasFocus = input->peerUuid() == controller->lastKeyboardInputRequester();
            input->setFocus(hasFocus);
            if (hasFocus)
                controller->setCurrentFocusItem(input->peerUuid());
        }
    }
}

} // Input

} // Qt3D

QT_END_NAMESPACE
