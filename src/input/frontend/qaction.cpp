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

#include "qaction.h"
#include "qaction_p.h"
#include <Qt3DCore/private/qnode_p.h>
#include <Qt3DCore/qnodepropertychange.h>
#include <Qt3DCore/qnodecreatedchange.h>
#include <Qt3DInput/qabstractactioninput.h>

QT_BEGIN_NAMESPACE

namespace Qt3DInput {

/*!
    \class Qt3DInput::QActionInput
    \inmodule Qt3DInput
    \inherits QAbstractActionInput
    \brief QActionInput stores Device and Keys used to trigger an input event.
    \since 5.7
*/

/*!
    \qmltype Action
    \inqmlmodule Qt3D.Input
    \instantiates Qt3DInput::QAction
    \brief QML frontend for the Qt3DInput::QAction C++ class.

    Links a set of QAbstractActionInputs that trigger the same event.
    \since 5.7
*/

/*!
    Constructs a new QAction instance with parent \a parent.
 */
QAction::QAction(Qt3DCore::QNode *parent)
    : Qt3DCore::QNode(*new QActionPrivate(), parent)
{
}

/*!
  \fn QAction::nameChanged()

  This signal is emitted when the name of the Action is changed.
*/

/*!
  \qmlproperty QString Qt3D.Input::Action::name

  the name used to identify this action
*/

/*!
    \qmlsignal Qt3D.Input::Action::nameChanged()

    This signal is emitted when the name of the Action is changed.

    The corresponding handeler is \c onNameChanged
*/

bool QAction::isActive() const
{
    Q_D(const QAction);
    return d->m_active;
}

/*!
  \qmlproperty QQmlListProperty<Qt3DInput::QAbstractActionInput> Qt3D.Input::Action::inputs

  the list of QAbstractActionInput that must be triggered to trigger this Action.
*/

/*!
    Append QAbstractActionInput \a input to the list of inputs that can trigger this action.
 */
void QAction::addInput(QAbstractActionInput *input)
{
    Q_D(QAction);
    if (!d->m_inputs.contains(input)) {
        d->m_inputs.push_back(input);

        if (!input->parent())
            input->setParent(this);

        if (d->m_changeArbiter != Q_NULLPTR) {
            Qt3DCore::QNodePropertyChangePtr change(new Qt3DCore::QNodePropertyChange(Qt3DCore::NodeAdded, Qt3DCore::QSceneChange::Node, id()));
            change->setPropertyName("input");
            change->setValue(QVariant::fromValue(input->id()));
            d->notifyObservers(change);
        }
    }
}

/*!
    Remove QAbstractActionInput \a input to the list of inputs that can trigger this action.
 */
void QAction::removeInput(QAbstractActionInput *input)
{
    Q_D(QAction);
    if (d->m_inputs.contains(input)) {

        if (d->m_changeArbiter != Q_NULLPTR) {
            Qt3DCore::QNodePropertyChangePtr change(new Qt3DCore::QNodePropertyChange(Qt3DCore::NodeRemoved, Qt3DCore::QSceneChange::Node, id()));
            change->setPropertyName("input");
            change->setValue(QVariant::fromValue(input->id()));
            d->notifyObservers(change);
        }

        d->m_inputs.removeOne(input);
    }
}

/*!
    Returns the list of inputs that can trigger this action.
 */
QVector<QAbstractActionInput *> QAction::inputs() const
{
    Q_D(const QAction);
    return d->m_inputs;
}

void QAction::copy(const Qt3DCore::QNode *ref)
{
    QNode::copy(ref);
    const QAction *action = static_cast<const QAction *>(ref);
    Q_FOREACH (QAbstractActionInput *input, action->inputs())
        d_func()->m_inputs.append(qobject_cast<QAbstractActionInput *>(QNode::clone(input)));
}

void QAction::sceneChangeEvent(const Qt3DCore::QSceneChangePtr &change)
{
    Q_D(QAction);
    Qt3DCore::QNodePropertyChangePtr e = qSharedPointerCast<Qt3DCore::QNodePropertyChange>(change);
    if (e->type() == Qt3DCore::NodeUpdated && e->propertyName() == QByteArrayLiteral("active")) {
        d->setActive(e->value().toBool());
    }
}

Qt3DCore::QNodeCreatedChangeBasePtr QAction::createNodeCreationChange() const
{
    auto creationChange = Qt3DCore::QNodeCreatedChangePtr<QActionData>::create(this);
    auto &data = creationChange->data;
    data.inputIds = qIdsForNodes(inputs());
    return creationChange;
}

} // Qt3DInput

QT_END_NAMESPACE
