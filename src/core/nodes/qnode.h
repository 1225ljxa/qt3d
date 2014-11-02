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

#ifndef QT3D_QNODE_H
#define QT3D_QNODE_H

#include <QObject>
#include <Qt3DCore/qt3dcore_global.h>
#include <Qt3DCore/qnodeuuid.h>
#include <Qt3DCore/qscenechange.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

class QNode;
class QNodePrivate;
class QEntity;
class QAspectEngine;
class QSceneInterface;

typedef QList<QNode *> NodeList;
typedef QSharedPointer<QNode> QNodePtr;

#define QT3D_CLONEABLE(Class)                \
    QNode *doClone() const Q_DECL_OVERRIDE { \
        Class *clone = new Class;            \
        clone->copy(this);                   \
        return clone;                        \
    }

class QT3DCORESHARED_EXPORT QNode : public QObject
{
    Q_OBJECT
public:
    explicit QNode(QNode *parent = 0);
    ~QNode();

    const QNodeUuid uuid() const;
    QNode *parentNode() const;

    bool notificationsBlocked() const;
    bool blockNotifications(bool block);

protected:
    QNode(QNodePrivate &dd, QNode *parent = 0);
    virtual void copy(const QNode *ref);
    virtual void sceneChangeEvent(const QSceneChangePtr &change);
    bool event(QEvent *e) Q_DECL_OVERRIDE;

private:
    Q_DECLARE_PRIVATE(QNode)
    Q_PRIVATE_SLOT(d_func(), void _q_onNodePropertyChanged())
    virtual QNode *doClone() const = 0;

    friend class QAspectEngine;
    friend class QPostman;
    friend class QScene;
};

} // namespace Qt3D

QT_END_NAMESPACE

Q_DECLARE_METATYPE(Qt3D::QNode *)

#endif
