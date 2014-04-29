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

#include "qmlentity.h"
#include <QEvent>

QT_BEGIN_NAMESPACE

namespace Qt3D {

QmlEntity::QmlEntity(Node *parent) : Entity(parent)
{

}

QQmlListProperty<Component> QmlEntity::componentList()
{
    return QQmlListProperty<Qt3D::Component>(this, 0,
                                             QmlEntity::qmlAppendComponent,
                                             QmlEntity::qmlComponentsCount,
                                             QmlEntity::qmlComponentAt,
                                             QmlEntity::qmlClearComponents);
}

void QmlEntity::qmlAppendComponent(QQmlListProperty<Component> *list, Component *comp)
{
    if (comp == Q_NULLPTR)
        return;
    QmlEntity *self = static_cast<QmlEntity *>(list->object);
    self->addComponent(comp);
}

Component *QmlEntity::qmlComponentAt(QQmlListProperty<Component> *list, int index)
{
    QmlEntity *self = static_cast<QmlEntity *>(list->object);
    return self->components().at(index);
}

int QmlEntity::qmlComponentsCount(QQmlListProperty<Component> *list)
{
    QmlEntity *self = static_cast<QmlEntity *>(list->object);
    return self->components().count();
}

void QmlEntity::qmlClearComponents(QQmlListProperty<Component> *list)
{
    QmlEntity *self = static_cast<QmlEntity *>(list->object);
    ComponentList components = self->components();
    Q_FOREACH (Component *comp, components) {
        self->removeComponent(comp);
//        delete comp;
    }
}

} // Qt3D

QT_END_NAMESPACE
