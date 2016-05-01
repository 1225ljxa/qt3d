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

#include "qnode.h"
#include "qnode_p.h"

#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qnodepropertychange.h>
#include <Qt3DCore/qnodeaddedpropertychange.h>
#include <Qt3DCore/qnoderemovedpropertychange.h>
#include <Qt3DCore/qnodedestroyedchange.h>
#include <Qt3DCore/qaspectengine.h>
#include <Qt3DCore/private/qdestructionidandtypecollector_p.h>
#include <Qt3DCore/private/qscene_p.h>
#include <Qt3DCore/private/qpostman_p.h>
#include <QEvent>
#include <QChildEvent>
#include <QMetaObject>
#include <QMetaProperty>
#include <Qt3DCore/QComponent>
#include <Qt3DCore/private/corelogging_p.h>
#include <Qt3DCore/private/qnodecreatedchangegenerator_p.h>
#include <Qt3DCore/private/qnodevisitor_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3DCore {

QHash<QNodeId, QNode *> QNodePrivate::m_clonesLookupTable = QHash<QNodeId, QNode *>();
const bool QNodePrivate::ms_useCloning = !qEnvironmentVariableIsSet("QT3D_NO_CLONE");

QNodePrivate::QNodePrivate()
    : QObjectPrivate()
    , m_changeArbiter(nullptr)
    , m_typeInfo(nullptr)
    , m_scene(nullptr)
    , m_id(QNodeId::createId())
    , m_blockNotifications(false)
    , m_hasBackendNode(false)
    , m_enabled(true)
    , m_propertyChangesSetup(false)
    , m_signals(this)
{
}

// Called by QNodePrivate::ctor or setParent  (main thread)
void QNodePrivate::_q_addChild(QNode *childNode)
{
    Q_ASSERT(childNode);
    Q_ASSERT_X(childNode->parent() == q_func(), Q_FUNC_INFO,  "not a child of this node");

    if (!m_scene)
        return;

    // We need to send a QNodeAddedChange to the backend

    // We notify the backend that we have a new child
    if (m_changeArbiter != nullptr) {
        const auto change = QNodeAddedPropertyChangePtr::create(m_id, childNode);
        change->setPropertyName("children");
        notifyObservers(change);
    }

    // Update the scene
    // TODO: Fold this into the QNodeCreatedChangeGenerator so we don't have to
    // traverse the sub tree three times!
    QNodeVisitor visitor;
    visitor.traverse(childNode, this, &QNodePrivate::addEntityComponentToScene);
}

// Called by setParent or cleanup (main thread) (could be other thread if created on the backend in a job)
void QNodePrivate::_q_removeChild(QNode *childNode)
{
    Q_ASSERT(childNode);
    Q_ASSERT_X(childNode->parent() == q_func(), Q_FUNC_INFO, "not a child of this node");

    // We notify the backend that we lost a child
    if (m_changeArbiter != nullptr) {
        const auto change = QNodeRemovedPropertyChangePtr::create(m_id, childNode);
        change->setPropertyName("children");
        notifyObservers(change);
    }
}

// Note: should never be called from the ctor directly as the type may not be fully
// created yet
void QNodePrivate::_q_setParentHelper(QNode *parent)
{
    Q_Q(QNode);
    QNode *oldParentNode = q->parentNode();

    // If we had a parent, we let him know that we are about to change
    // parent
    if (oldParentNode) {
        QNodePrivate::get(oldParentNode)->_q_removeChild(q);

        // If we have an old parent but the new parent is null
        // the backend node needs to be destroyed
        if (!parent) {
            // Tell the backend we are about to be destroyed
            if (m_hasBackendNode) {
                const QDestructionIdAndTypeCollector collector(q);
                const auto destroyedChange = QNodeDestroyedChangePtr::create(q, collector.subtreeIdsAndTypes());
                notifyObservers(destroyedChange);
            }

            // We unset the scene from the node as its backend node was/is about to be destroyed
            QNodeVisitor visitor;
            visitor.traverse(q, oldParentNode->d_func(), &QNodePrivate::unsetSceneHelper);
        }
    }

    // Basically QObject::setParent but for QObjectPrivate
    QObjectPrivate::setParent_helper(parent);
    QNode *newParentNode = q->parentNode();

    if (newParentNode) {
        // If we had no parent but are about to set one,
        // we need to send a QNodeCreatedChangeGenerator
        if (!oldParentNode) {
            QNodePrivate *newParentPrivate = QNodePrivate::get(newParentNode);

            // Set the scene helper / arbiter
            if (newParentPrivate->m_scene) {
                QNodeVisitor visitor;
                visitor.traverse(q, newParentNode->d_func(), &QNodePrivate::setSceneHelper);
            }

            if (m_changeArbiter) {
                QNodeCreatedChangeGenerator generator(q);
                const auto creationChanges = generator.creationChanges();
                for (const auto &change : creationChanges)
                    notifyObservers(change);
            }
        }

        // If we have a valid new parent, we let him know that we are its child
        QNodePrivate::get(newParentNode)->_q_addChild(q);
    }
}

void QNodePrivate::registerNotifiedProperties()
{
    Q_Q(QNode);
    if (m_propertyChangesSetup)
        return;

    const int offset = QNode::staticMetaObject.propertyOffset();
    const int count = q->metaObject()->propertyCount();

    for (int index = offset; index < count; index++)
        m_signals.connectToPropertyChange(q, index);

    m_propertyChangesSetup = true;
}

void QNodePrivate::unregisterNotifiedProperties()
{
    Q_Q(QNode);
    if (!m_propertyChangesSetup)
        return;

    const int offset = QNode::staticMetaObject.propertyOffset();
    const int count = q->metaObject()->propertyCount();

    for (int index = offset; index < count; index++)
        m_signals.disconnectFromPropertyChange(q, index);

    m_propertyChangesSetup = false;
}

void QNodePrivate::propertyChanged(int propertyIndex)
{
    // Bail out early if we can to avoid the cost below
    if (m_blockNotifications)
        return;

    Q_Q(QNode);

    const QMetaProperty property = q->metaObject()->property(propertyIndex);

    const QVariant data = property.read(q);
    if (data.canConvert<QNode*>()) {
        const QNode * const node = data.value<QNode*>();
        const QNodeId id = node ? node->id() : QNodeId();
        notifyPropertyChange(property.name(), QVariant::fromValue(id));
    } else {
        notifyPropertyChange(property.name(), data);
    }
}

/*!
    \internal
    Recursively sets and adds the nodes in the subtree of base node \a root to the scene.
    Also takes care of connecting Components and Entities together in the scene.
 */
void QNodePrivate::setSceneHelper(QNode *root)
{
    // Sets the scene
    root->d_func()->setScene(m_scene);
    // addObservable sets the QChangeArbiter
    m_scene->addObservable(root);

    // We also need to handle QEntity <-> QComponent relationships
    if (QComponent *c = qobject_cast<QComponent *>(root)) {
        const QVector<QEntity *> entities = c->entities();
        for (QEntity *entity : entities) {
            if (!m_scene->hasEntityForComponent(c->id(), entity->id())) {
                if (!c->isShareable() && !m_scene->entitiesForComponent(c->id()).isEmpty())
                    qWarning() << "Trying to assign a non shareable component to more than one Entity";
                m_scene->addEntityForComponent(c->id(), entity->id());
            }
        }
    }
}

/*!
    \internal

    Recursively unsets and remove nodes in the subtree of base node \a root from
    the scene. Also takes care of removing Components and Entities connections.
 */
void QNodePrivate::unsetSceneHelper(QNode *root)
{
    // We also need to handle QEntity <-> QComponent relationships removal
    if (QComponent *c = qobject_cast<QComponent *>(root)) {
        const QVector<QEntity *> entities = c->entities();
        for (QEntity *entity : entities) {
            if (m_scene)
                m_scene->removeEntityForComponent(c->id(), entity->id());
        }
    }

    if (m_scene != nullptr)
        m_scene->removeObservable(root);
    root->d_func()->setScene(nullptr);
}

/*!
    \internal
 */
void QNodePrivate::addEntityComponentToScene(QNode *root)
{
    if (QEntity *e = qobject_cast<QEntity *>(root)) {
        const auto components = e->components();
        for (QComponent *c : components) {
            if (!m_scene->hasEntityForComponent(c->id(), e->id()))
                m_scene->addEntityForComponent(c->id(), e->id());
        }
    }
}

/*!
    \internal
 */
// Called in the main thread by QScene -> following QEvent::childAdded / addChild
void QNodePrivate::setArbiter(QLockableObserverInterface *arbiter)
{
    if (m_changeArbiter && m_changeArbiter != arbiter)
        unregisterNotifiedProperties();
    m_changeArbiter = static_cast<QAbstractArbiter *>(arbiter);
    if (m_changeArbiter)
        registerNotifiedProperties();
}

/*!
    Called when one or more backend aspects sends a notification \a change to the
    current Qt3DCore::QNode instance.

    \note This method should be reimplemented in your subclasses to properly
    handle the \a change.
*/
void QNode::sceneChangeEvent(const QSceneChangePtr &change)
{
    Q_UNUSED(change);
    qWarning() << Q_FUNC_INFO << "sceneChangeEvent should have been subclassed";
}

/*!
    \internal
 */
void QNodePrivate::setScene(QScene *scene)
{
    if (m_scene != scene)
        m_scene = scene;
}

/*!
    \internal
 */
QScene *QNodePrivate::scene() const
{
    return m_scene;
}

/*!
    \internal
 */
void QNodePrivate::notifyPropertyChange(const char *name, const QVariant &value)
{
    // Bail out early if we can to avoid operator new
    if (m_blockNotifications)
        return;

    QNodePropertyChangePtr e(new QNodePropertyChange(NodeUpdated, QSceneChange::Node, m_id));
    e->setPropertyName(name);
    e->setValue(value);
    notifyObservers(e);
}

/*!
    \internal
 */
// Called by the main thread
void QNodePrivate::notifyObservers(const QSceneChangePtr &change)
{
    Q_ASSERT(change);

    // Don't send notifications if we are blocking
    if (m_blockNotifications && change->type() == NodeUpdated)
        return;

    if (m_changeArbiter != nullptr) {
        QAbstractPostman *postman = m_changeArbiter->postman();
        if (postman != nullptr)
            postman->notifyBackend(change);
    }
}

// Inserts this tree into the main Scene tree.
// Needed when SceneLoaders provide a cloned tree from the backend
// and need to insert it in the main scene tree
// QNode *root;
// QNode *subtree;
// QNodePrivate::get(root)->insertTree(subtree);

/*!
    \internal
 */
void QNodePrivate::insertTree(QNode *treeRoot, int depth)
{
    if (m_scene != nullptr) {
        treeRoot->d_func()->setScene(m_scene);
        m_scene->addObservable(treeRoot);
    }

    for (QObject *c : treeRoot->children()) {
        QNode *n = nullptr;
        if ((n = qobject_cast<QNode *>(c)) != nullptr)
            insertTree(n, depth + 1);
    }

    if (depth == 0)
        treeRoot->setParent(q_func());
}

/*!
    \internal
 */
QNodePrivate *QNodePrivate::get(QNode *q)
{
    return q->d_func();
}

/*!
    \internal
 */
void QNodePrivate::nodePtrDeleter(QNode *q)
{
    QObject *p = q->parent();
    if (p == nullptr)
        p = q;
    p->deleteLater();
}


/*!
    \class Qt3DCore::QNode
    \inherits QObject

    \inmodule Qt3DCore
    \since 5.5

    \brief QNode is the base class of all Qt3D node classes used to build a
    Qt3D scene.

    The owernship of QNode is determined by the QObject parent/child
    relationship between nodes. By itself, a QNode has no visual appearance
    and no particular meaning, it is there as a way of building a node based tree
    structure.

    The parent of a QNode instance can only be another QNode instance.

    Each QNode instance has a unique id that allows it to be recognizable
    from other instances.

    When properties are defined on a QNode subclass, their NOTIFY signal
    will automatically generate notifications that the Qt3D backend aspects will
    receive.

    When subclassing QNode, make sure to call QNode::cleanup() from your
    subclass's destructor to ensure proper notification to backend aspects.
    Faiure to do so will result in crashes when one of your QNode subclass
    instance is eventually destroyed.

    \sa QEntity, QComponent
*/

/*!
    \fn QNodeId qIdForNode(QNode *node)
    \relates Qt3DCore::QNode
    \return node id for \a node.
*/

/*!
    \fn QNodeIdVector qIdsForNodes(const T &nodes)
    \relates Qt3DCore::QNode
    \return vector of node ids for \a nodes.
*/

/*!
     Creates a new QNode instance with parent \a parent.

     \note The backend aspects will be notified that a QNode instance is
     part of the scene only if it has a parent; unless this is the root node of
     the Qt3D scene.

     \sa setParent()
*/
QNode::QNode(QNode *parent)
    : QObject(*new QNodePrivate)
{
    // We need to add ourselves with the parent if it is valid
    // This will notify the backend about the new child
    if (parent/* && QNodePrivate::get(parent)->m_changeArbiter != nullptr*/) {
        // This needs to be invoked  only after the QNode has been fully constructed
        QMetaObject::invokeMethod(this, "_q_setParentHelper", Qt::QueuedConnection, Q_ARG(Qt3DCore::QNode*, parent));
    }
}

/*! \internal */
QNode::QNode(QNodePrivate &dd, QNode *parent)
    : QObject(dd)
{
    // We need to add ourselves with the parent if it is valid
    // This will notify the backend about the new child
    if (parent/* && QNodePrivate::get(parent)->m_changeArbiter != nullptr*/) {
        // This needs to be invoked  only after the QNode has been fully constructed
        QMetaObject::invokeMethod(this, "_q_setParentHelper", Qt::QueuedConnection, Q_ARG(Qt3DCore::QNode*, parent));
    }
}

QNode::~QNode()
{
    // If we have a parent it makes sense to let it know we are about to be destroyed.
    // This in turn triggers the deletion of the corresponding backend nodes for the
    // subtree rooted at this QNode.
    setParent(Q_NODE_NULLPTR);
}

/*!
    Returns the id that uniquely identifies the QNode instance.
*/
const QNodeId QNode::id() const
{
    Q_D(const QNode);
    return d->m_id;
}

/*!
    \property Qt3DCore::QNode::parent

    Holds the immediate QNode parent, or null if the node has no parent.

    Setting the parent will notify the backend aspects about current QNode
    instance's parent change.

    \note if \a parent happens to be null, this will actually notify that the
    current QNode instance was removed from the scene.
*/
QNode *QNode::parentNode() const
{
    return qobject_cast<QNode*>(parent());
}

/*!
    Returns \c true if aspect notifications are blocked; otherwise returns \c false.
    By default, notifications are \e not blocked.

    \sa blockNotifications()
*/
bool QNode::notificationsBlocked() const
{
    Q_D(const QNode);
    return d->m_blockNotifications;
}

/*!
    If \a block is \c true, property change notifications sent by this object
    to aspects are blocked. If \a block is \c false, no such blocking will occur.

    The return value is the previous value of notificationsBlocked().

    Note that the other notification types will be sent even if the
    notifications for this object have been blocked.

    \sa notificationsBlocked()
*/
bool QNode::blockNotifications(bool block)
{
    Q_D(QNode);
    bool previous = d->m_blockNotifications;
    d->m_blockNotifications = block;
    return previous;
}

// Note: should never be called from the ctor directly as the type may not be fully
// created yet
void QNode::setParent(QNode *parent)
{
    Q_D(QNode);
    if (parentNode() == parent)
        return;
    d->_q_setParentHelper(parent);

    // Block notifications as we want to let the _q_setParentHelper
    // manually handle them
    const bool blocked = blockNotifications(true);
    emit parentChanged(parent);
    blockNotifications(blocked);
}

/*!
    \typedef Qt3DCore::QNodePtr
    \relates Qt3DCore::QNode

    A shared pointer for QNode.
*/
/*!
    \typedef Qt3DCore::QNodeVector
    \relates Qt3DCore::QNode

    List of QNode pointers.
*/

/*!
 * Returns a list filled with the QNode children of the current
 * QNode instance.
 */
QNodeVector QNode::childNodes() const
{
    QNodeVector nodeChildrenList;
    const QObjectList &objectChildrenList = QObject::children();
    nodeChildrenList.reserve(objectChildrenList.size());

    for (QObject *c : objectChildrenList) {
        if (QNode *n = qobject_cast<QNode *>(c))
            nodeChildrenList.push_back(n);
    }

    return nodeChildrenList;
}
void QNode::setEnabled(bool isEnabled)
{
    Q_D(QNode);

    if (d->m_enabled == isEnabled)
        return;

    d->m_enabled = isEnabled;
    emit enabledChanged(isEnabled);
}

/*!
    \property Qt3DCore::QNode::enabled

    Holds the QNode enabled flag.
    By default a QNode is always enabled.

    \note the interpretation of what enabled means is aspect-dependent. Even if
    enabled is set to \c false, some aspects may still consider the node in
    some manner. This is documented on a class by class basis.
*/
bool QNode::isEnabled() const
{
    Q_D(const QNode);
    return d->m_enabled;
}

QNodeCreatedChangeBasePtr QNode::createNodeCreationChange() const
{
    // Uncomment this when implementing new frontend and backend types.
    // Any classes that don't override this function will be noticeable here.
    // Note that some classes actually don't need to override as they have
    // no additional data to send. In those cases this default implementation
    // is perfectly fine.
    // const QMetaObject *mo = metaObject();
    // qDebug() << Q_FUNC_INFO << mo->className();
    return QNodeCreatedChangeBasePtr::create(this);
}

} // namespace Qt3DCore

QT_END_NAMESPACE

#include "moc_qnode.cpp"
