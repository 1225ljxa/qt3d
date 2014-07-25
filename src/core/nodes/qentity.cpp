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

#include "qentity.h"
#include "qentity_p.h"
#include "qcomponent.h"
#include "qabstracttransform.h"
#include "qmatrixtransform.h"

#include <Qt3DCore/qscenepropertychange.h>
#include <QMetaObject>
#include <QMetaProperty>
#include "corelogging.h"

QT_BEGIN_NAMESPACE

namespace Qt3D {

QEntity::QEntity(QNode *parent)
    : QNode(*new QEntityPrivate(this), parent)
{
}

QEntity::~QEntity()
{
    // If all children are removed
    // That includes the components that are parented by this entity
//    removeAllComponents();
}

QEntity::QEntity(QEntityPrivate &dd, QNode *parent)
    : QNode(dd, parent)
{
}

QList<QComponent *> QEntity::components() const
{
    Q_D(const QEntity);
    return d->m_components;
}

void QEntity::addComponent(QComponent *comp)
{
    Q_D(QEntity);
    Q_CHECK_PTR( comp );
    qCDebug(Nodes) << Q_FUNC_INFO << comp;
    Q_ASSERT(d->m_components.count(comp) == 0);
    d->m_components.append(comp);
    // We only set the Entity as the Component's parent when it has no parent
    // This will be the case mostly on C++ but rarely in QML
    if (!comp->parent())
        addChild(comp);

    QScenePropertyChangePtr propertyChange(new QScenePropertyChange(ComponentAdded, this));
    propertyChange->setPropertyName(QByteArrayLiteral("component"));
    propertyChange->setValue(QVariant::fromValue(comp));
    notifyObservers(propertyChange);
}

// As in most cases Components are children of the Entity
// They shouldn't therefore also be called in the removeAllChildren of QNode
// How to handle QML inline declaration however ?
void QEntity::removeComponent(QComponent *comp)
{
    Q_CHECK_PTR(comp);
    qCDebug(Nodes) << Q_FUNC_INFO << comp;
    Q_D(QEntity);
    d->m_components.removeOne(comp);
    QScenePropertyChangePtr propertyChange(new QScenePropertyChange(ComponentRemoved, this));
    propertyChange->setValue(QVariant::fromValue(comp));
    propertyChange->setPropertyName(QByteArrayLiteral("component"));
    notifyObservers(propertyChange);
}

void QEntity::removeAllComponents()
{
    Q_D(const QEntity);
    Q_FOREACH (QComponent *comp, d->m_components)
        removeComponent(comp);
}

bool QEntity::isEnabled() const
{
    Q_D(const QEntity);
    return d->m_enabled;
}

void QEntity::setEnabled(bool on)
{
    Q_D(QEntity);
    if (d->m_enabled != on) {
        d->m_enabled = on;
        emit enabledChanged();
    }
}

QEntity *QEntity::parentEntity()
{
    QNode *parentNode = QNode::parentNode();
    QEntity *parentEntity = qobject_cast<QEntity*>(parentNode);

    while (parentEntity == Q_NULLPTR && parentNode != Q_NULLPTR) {
        parentNode = parentNode->parentNode();
        parentEntity = qobject_cast<QEntity*>(parentNode);
    }
    return parentEntity;
}

QEntity *QEntity::asEntity()
{
    return this;
}

} // namespace Qt3D

QT_END_NAMESPACE
