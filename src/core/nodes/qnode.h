/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
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

#ifndef QT3DCORE_QNODE_H
#define QT3DCORE_QNODE_H

#include <QObject>
#include <Qt3DCore/qt3dcore_global.h>
#include <Qt3DCore/qnodeid.h>
#include <Qt3DCore/qscenechange.h>
#include <Qt3DCore/qnodecreatedchange.h>
#include <Qt3DCore/qabstractnodefactory.h>

#define Q_NODE_NULLPTR static_cast<Qt3DCore::QNode *>(Q_NULLPTR)

QT_BEGIN_NAMESPACE

namespace Qt3DCore {

class QNode;
class QNodePrivate;
class QEntity;
class QAspectEngine;

typedef QVector<QNode *> QNodeVector;
typedef QSharedPointer<QNode> QNodePtr;

// Each QNode subclass should call QNode::cleanup in it dtor
// QNode::cleanup checks that a flags wasn't set to true,
// sets it to true and sends a clone to the backend

class QT3DCORESHARED_EXPORT QNode : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Qt3DCore::QNode *parent READ parentNode WRITE setParent NOTIFY parentChanged)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
public:
    explicit QNode(QNode *parent = Q_NULLPTR);
    virtual ~QNode();

    const QNodeId id() const;
    QNode *parentNode() const;

    bool notificationsBlocked() const;
    bool blockNotifications(bool block);

    QNodeVector childNodes() const;

    bool isEnabled() const;

public Q_SLOTS:
    void setParent(QNode *parent);
    void setEnabled(bool isEnabled);

Q_SIGNALS:
    void parentChanged(QObject *parent);
    void enabledChanged(bool enabled);

protected:
    QNode(QNodePrivate &dd, QNode *parent = Q_NULLPTR);
    virtual void sceneChangeEvent(const QSceneChangePtr &change);

private:
    Q_DECLARE_PRIVATE(QNode)
    // TODO: Make this pure virtual once all classes have been adapted
    virtual QNodeCreatedChangeBasePtr createNodeCreationChange() const;

    // We only want setParent(QNode *) to be callable
    // when dealing with QNode objects
    void setParent(QObject *) Q_DECL_EQ_DELETE;

    Q_PRIVATE_SLOT(d_func(), void _q_addChild(Qt3DCore::QNode *))
    Q_PRIVATE_SLOT(d_func(), void _q_removeChild(Qt3DCore::QNode *))

    friend class QAspectEngine;
    friend class QAspectEnginePrivate;
    friend class QNodeCreatedChangeGenerator;
    friend class QPostman;
    friend class QScene;
};

inline QNodeId qIdForNode(QNode *node){ return node ? node->id() : QNodeId(); }

template<typename T>
inline QNodeIdVector qIdsForNodes(const T &nodes)
{
    QNodeIdVector ids;
    ids.reserve(nodes.size());
    for (const auto n : nodes)
        ids.push_back(n->id());
    return ids;
}

} // namespace Qt3DCore

QT_END_NAMESPACE

#endif
