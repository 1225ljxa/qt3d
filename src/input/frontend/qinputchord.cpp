/****************************************************************************
**
** Copyright (C) 2016 Klaralvdalens Datakonsult AB (KDAB).
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
****************************************************************************/

#include "qinputchord.h"
#include <Qt3DInput/private/qabstractaggregateactioninput_p.h>
#include <Qt3DInput/qabstractphysicaldevice.h>
#include <Qt3DInput/qabstractaggregateactioninput.h>

QT_BEGIN_NAMESPACE

namespace Qt3DInput {
/*!
    \class Qt3DInput::QInputChordPrivate
    \internal
*/
class QInputChordPrivate : public Qt3DInput::QAbstractAggregateActionInputPrivate
{
public:
    QInputChordPrivate()
        : Qt3DInput::QAbstractAggregateActionInputPrivate()
        , m_tolerance(0)
    {}

    int m_tolerance;
};

/*!
    \class Qt3DInput::QInputChord
    \inmodule Qt3DInput
    \inherits QAbstractAggregateActionInput
    \brief QInputChord represents a set of QAbstractActionInput's that must be triggerd at once.

    \since 5.7
*/

/*!
    \qmltype InputChord
    \inqmlmodule Qt3D.Input
    \inherits QAbstractAggregateActionInput
    \instantiates Qt3DInput::QInputChord
    \brief QML frontend for the Qt3DInput::QInputChord C++ class.

    Represents a set of QAbstractActionInput's that must be triggerd at once.
    \since 5.7
*/

/*!
    Constructs a new QInputChord with parent \a parent.
 */
QInputChord::QInputChord(Qt3DCore::QNode *parent)
    : Qt3DInput::QAbstractAggregateActionInput(*new QInputChordPrivate(), parent)
{

}

/*!
    Deletes the QInputChord instance.
 */
QInputChord::~QInputChord()
{
    QNode::cleanup();
}

/*!
  \qmlproperty int Qt3D.Input::InputChord::tollerance

  the time in milliseconds in which all QAbstractActionInput's in the input chord must triggered within.
*/

/*!
    Returns the time in which all QAbstractActionInput's in the input chord must triggered within.
    The time is in milliseconds
 */
int QInputChord::tolerance() const
{
    Q_D(const QInputChord);
    return d->m_tolerance;
}

/*!
    Sets the time in which all QAbstractActionInput's in the input chord must triggered within.
    The time is in milliseconds
 */
void QInputChord::setTolerance(int tolerance)
{
    Q_D(QInputChord);
    if (d->m_tolerance != tolerance) {
        d->m_tolerance = tolerance;
        emit toleranceChanged(tolerance);
    }
}

void QInputChord::copy(const Qt3DCore::QNode *ref)
{
    QAbstractAggregateActionInput::copy(ref);
    const QInputChord *input = static_cast<const QInputChord *>(ref);
    d_func()->m_tolerance = input->d_func()->m_tolerance;
}

} // Qt3DInput

QT_END_NAMESPACE
