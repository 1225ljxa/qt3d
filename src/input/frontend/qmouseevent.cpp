/****************************************************************************
**
** Copyright (C) 2015 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qmouseevent.h"

QT_BEGIN_NAMESPACE

namespace Qt3DInput {

// Notes:
// Maybe we should provide the world pos of the intersection
// The distance t along the segment line at which the intersection occurs
// Screen Pos / Mouse Pos / Viewport Pos
// The intersection Ray
// These can always be added in follow up commits once the input API takes shape

/*!
 * \class Qt3DInput::QMouseEvent
 * \inmodule Qt3DInput
 *
 * \brief The Qt3DCore::QMouseEvent contains parameters that describe a mouse event.
 *
 * Mouse events occur when a mouse button is pressed and the ray
 * traversing the view, originating from the mouse position intersects with one
 * or more elements of the scene.
 *
 * \since 5.5
 *
 * \sa QKeyEvent, QWheelEvent, QMouseInput
 *
 */

/*!
 * \fn int Qt3DInput::QMouseEvent::x() const
 *
 *  Returns the x position of the mouse event.
 */

/*!
 * \fn int Qt3DInput::QMouseEvent::y() const
 *
 *  Returns the y position of the mouse event.
 */

/*!
 * \fn bool Qt3DInput::QMouseEvent::isAccepted() const
 *
 *  Returns whether the event was accepted.
 */

/*!
 * \fn void Qt3DInput::QMouseEvent::setAccepted(bool accepted)
 *
 *  Sets the event as accepted if \a accepted is true.
 *
 * \note When an event is accepted, it will prevent further propagation to other
 * listeners.
 */

/*!
 * \fn QEvent::Type Qt3DInput::QMouseEvent::type() const
 *
 *  Returns the QEvent::Type of the event.
 */

/*!
 * Constructs a new QMouseEvent instance for the QMouseEvent \a e.
 */
QMouseEvent::QMouseEvent(const QT_PREPEND_NAMESPACE(QMouseEvent) &e)
    : QObject()
    , m_event(e)
{
}

QMouseEvent::~QMouseEvent()
{
}

/*!
 * Returns the mouse button of the mouse event.
 */
QMouseEvent::Buttons QMouseEvent::button() const
{
    switch (m_event.button()) {
    case Qt::MouseButton::LeftButton:
        return QMouseEvent::LeftButton;
    case Qt::MouseButton::RightButton:
        return QMouseEvent::RightButton;
    case Qt::MouseButton::MiddleButton:
        return QMouseEvent::MiddleButton;
    case Qt::MouseButton::BackButton:
        return QMouseEvent::BackButton;
    default:
        return QMouseEvent::NoButton;
    }
}

/*!
 * Returns a bitfield to be used to check for mouse buttons that may be
 * accompanying the mouse event.
 */
int QMouseEvent::buttons() const
{
   return m_event.buttons();
}

/*!
 * Returns the keyboard modifier that may be accompanying the mouse event.
 */
QMouseEvent::Modifiers QMouseEvent::modifier() const
{
    switch (m_event.modifiers()) {
    case Qt::ShiftModifier:
        return QMouseEvent::ShiftModifier;
    case Qt::ControlModifier:
        return QMouseEvent::ControlModifier;
    case Qt::AltModifier:
        return QMouseEvent::AltModifier;
    case Qt::MetaModifier:
        return QMouseEvent::MetaModifier;
    case Qt::KeypadModifier:
        return QMouseEvent::KeypadModifier;
    default:
        return QMouseEvent::NoModifier;
    }
}


/*!
 * \class Qt3DInput::QWheelEvent
 * \inmodule Qt3DInput
 *
 * \brief The QWheelEvent class contains parameters that describe a mouse wheel event.
 *
 * Mouse wheel events occur when the mouse is rotated.
 *
 * \since 5.5
 *
 * \sa QKeyEvent, QMouseEvent, QMouseInput
 *
 */

/*!
 * \fn int Qt3DInput::QWheelEvent::x() const
 *
 *  Returns the x position of the mouse event.
 */

/*!
 * \fn int Qt3DInput::QWheelEvent::y() const
 *
 *  Returns the x position of the mouse event.
 */

/*!
 * \fn QPoint Qt3DInput::QWheelEvent::angleDelta() const
 *
 * Returns the distance that the wheel is rotated, in eighths of a degree. A
 * positive value indicates that the wheel was rotated forward (away from the
 * user), a negative value indicates the wheel was rotated backward (toward the
 * user).
 */

/*!
 * \fn bool Qt3DInput::QWheelEvent::isAccepted() const
 *
 *  Returns whether the event was accepted.
 */

/*!
 * \fn void Qt3DInput::QWheelEvent::setAccepted(bool accepted)
 *
 *  Sets the event as accepted if \a accepted is true.
 *
 * \note When an event is accepted, it will prevent further propagation to other
 * listeners.
 */

/*!
 * \fn QEvent::Type Qt3DInput::QWheelEvent::type() const
 *
 *  Returns the QEvent::Type of the event.
 */

/*!
 * Constructs a new QWheelEvent instance from the QWheelEvent \a e.
 */
QWheelEvent::QWheelEvent(const QT_PREPEND_NAMESPACE(QWheelEvent) &e)
    : QObject()
    , m_event(e)
{
}

QWheelEvent::~QWheelEvent()
{
}

/*!
 * Returns a bitfield to be used to check for mouse buttons that may be
 * accompanying the wheel event.
 */
int QWheelEvent::buttons() const
{
    return m_event.buttons();
}

/*!
 * Returns the keyboard modifier that may be accompanying the wheel event.
 */
QWheelEvent::Modifiers QWheelEvent::modifier() const
{
    switch (m_event.modifiers()) {
    case Qt::ShiftModifier:
        return QWheelEvent::ShiftModifier;
    case Qt::ControlModifier:
        return QWheelEvent::ControlModifier;
    case Qt::AltModifier:
        return QWheelEvent::AltModifier;
    case Qt::MetaModifier:
        return QWheelEvent::MetaModifier;
    case Qt::KeypadModifier:
        return QWheelEvent::KeypadModifier;
    default:
        return QWheelEvent::NoModifier;
    }
}

} // namespace Qt3DInput

QT_END_NAMESPACE
