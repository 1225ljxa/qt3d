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

#ifndef QT3D_ENTITY_H
#define QT3D_ENTITY_H

#include "node.h"
#include "qt3dcore_global.h"

#include <QMatrix4x4>
#include <QUuid>

namespace Qt3D {

class AbstractTransform;
class Component;

typedef QList<Component*> ComponentList;

class QT3DCORESHARED_EXPORT Entity : public Node
{
    Q_OBJECT

    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)

public:
    explicit Entity(Node *parent = 0);

    QUuid uuid() const { return m_uuid; }

    ComponentList components() const;

    template <class T>
    QList<T*> componentsOfType() const
    {
        QList<T*> result;
        foreach (Component* comp, components()) {
            T* i = qobject_cast<T*>(comp);
            if (i)
                result.append(i);
        }

        return result;
    }

    template <class T>
    static T* findComponentInTree(Node* root)
    {
        if (!root)
            return Q_NULLPTR;

        if (root->asEntity()) {
            foreach (Component* comp, root->asEntity()->components()) {
                T* i = qobject_cast<T*>(comp);
                if (i)
                    return i;
            } // of component iteration
        } // of is-entity

        foreach (Node* child, root->children()) {
            T* i = findComponentInTree<T>(child);
            if (i)
                return i;
        } // of child nodes iteration

        return Q_NULLPTR;
    }

    template <class T>
    static T* findEntityInTree(Node* root)
    {
        if (!root)
            return Q_NULLPTR;

        if (root->asEntity()) {
            foreach (Node* child, root->children()) {
                if (!qobject_cast<Entity*>(child))
                    continue;
                T* i = qobject_cast<T*>(child);
                if (i)
                    return i;
            } // of child iteration
        } // of is-entity

        foreach (Node* child, root->children()) {
            T* i = findEntityInTree<T>(child);
            if (i)
                return i;
        } // of child nodes iteration

        return Q_NULLPTR;
    }

    void addComponent(Component *comp);
    void removeComponent(Component *comp);

    bool isEnabled() const;
    void setEnabled(bool on);

    virtual Entity* asEntity();

    Entity* parentEntity();

    // TEMPORARY - should be driven by an Update traversal in the core
    void update();
signals:
    void enabledChanged();

private:
    ComponentList m_components;
    bool m_visible;

    QUuid m_uuid;

    mutable QMatrix4x4 m_matrix;
    QMatrix4x4 m_sceneMatrix;

    // TODO: Is a bool enough here or do we need additional states for entities?
    // Perhaps aboutToBeDeleted would be useful?
    bool m_enabled;
};

} // namespace Qt3D

#endif // QT3D_ENTITY_H
