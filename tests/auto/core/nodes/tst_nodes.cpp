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

#include <QtTest/QtTest>
#include <Qt3DCore/qnode.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DCore/qcomponent.h>
#include <Qt3DCore/qscene.h>

class tst_Nodes : public QObject
{
    Q_OBJECT
public:
    tst_Nodes() : QObject() {}
    ~tst_Nodes() {}

private slots:
    void defaultNodeConstruction();
    void defaultComponentConstruction();
    void defaultEntityConstrution();
    void appendChildNodesToNode();
    void removingChildNodesFromNode();
    void appendingChildEntitiesToNode();
    void removingChildEntitiesFromNode();
    void appendingComponentsToEntity();
    void removingComponentsFromEntity();
    void checkCloning();
    void checkDestruction();
};

class MyQNode : public Qt3D::QNode
{
    Q_OBJECT
public:
    explicit MyQNode(Qt3D::QNode *parent = 0) : QNode(parent)
    {}

    void copy(const Qt3D::QNode *ref)
    {
        QNode::copy(ref);
        if (qobject_cast<const MyQNode *>(ref) != Q_NULLPTR) {
            m_customProperty = qobject_cast<const MyQNode *>(ref)->customProperty();
        }
    }

    void setCustomProperty(const QString &s) { m_customProperty = s; }
    QString customProperty() const { return m_customProperty; }

protected:
    Qt3D::QNode *doClone(bool isClone = true) const Q_DECL_OVERRIDE
    {
        MyQNode *clone = new MyQNode();
        clone->copy(this);
        //        clone->d_func()->m_isClone = isClone;
        return clone;
    }

    QString m_customProperty;

};

class MyQComponent : public Qt3D::QComponent
{
    Q_OBJECT
public:
    explicit MyQComponent(Qt3D::QNode *parent = 0) : QComponent(parent)
    {}

    // QNode interface
protected:
    Qt3D::QNode *doClone(bool isClone = true) const Q_DECL_OVERRIDE
    {
        MyQComponent *clone = new MyQComponent();
        clone->copy(this);
//        clone->d_func()->m_isClone = isClone;
        return clone;
    }
};


void tst_Nodes::defaultNodeConstruction()
{
    MyQNode *node = new MyQNode();
    MyQNode node2(node);

    QVERIFY(node != Q_NULLPTR);
    QVERIFY(node->children().isEmpty());
    QVERIFY(node2.parent() == node);
    node->addChild(&node2);
    QVERIFY(!node->children().isEmpty());
    QVERIFY(node2.children().isEmpty());
}

void tst_Nodes::defaultComponentConstruction()
{
    MyQComponent *comp = new MyQComponent();
    MyQComponent comp2(comp);

    QVERIFY(comp != Q_NULLPTR);
    QVERIFY(comp2.parent() == comp);
}

void tst_Nodes::defaultEntityConstrution()
{
    Qt3D::QEntity *entity = new Qt3D::QEntity();
    Qt3D::QEntity entity2(entity);

    QVERIFY(entity->components().isEmpty());
    QVERIFY(entity2.components().isEmpty());
    QVERIFY(entity2.parent() == entity);
}

void tst_Nodes::appendChildNodesToNode()
{
    MyQNode *node = new MyQNode();

    for (int i = 0; i < 10; i++) {
        MyQNode *child = new MyQNode();
        QVERIFY(child->parent() == Q_NULLPTR);
        node->addChild(child);
        QVERIFY(child->parent() == node);
        QVERIFY(child->parentNode() == node);
    }
    QVERIFY(node->children().count() == 10);
    for (int i = 0; i < 10; i++) {
        MyQNode *child = new MyQNode();
        node->addChild(child);
        QVERIFY(child->parent() == node);
        QVERIFY(child->parentNode() == node);
    }
    QVERIFY(node->children().count() == 20);
    Qt3D::QNode *child = node->children().first();
    Qt3D::QNode *parent = node;
    for (int i = 0; i < 10; i++) {
        QVERIFY(child->parent() == parent);
        QVERIFY(child->parentNode() == parent);
        child->addChild(new MyQNode());
        parent = child;
        child = child->children().first();
    }
    QVERIFY(node->children().count() == 20);
    child = node->children().first();
    parent = node;
    for (int i = 0; i < 10; i++) {
        QVERIFY(child->parent() == parent);
        QVERIFY(child->parentNode() == parent);
        QVERIFY(child->children().count() == 1);
        parent = child;
        child = child->children().first();
    }
    QVERIFY(child->children().count() == 0);
}

void tst_Nodes::removingChildNodesFromNode()
{
    MyQNode *root = new MyQNode();
    Qt3D::QNode *child = new MyQNode();

    root->addChild(child);
    QVERIFY(root->children().count() == 1);
    root->removeChild(child);
    QVERIFY(root->children().count() == 0);
    QVERIFY(child->parent() == Q_NULLPTR);

    for (int i = 0; i < 10; i++) {
        root->addChild(new MyQNode());
    }
    QVERIFY(root->children().count() == 10);
    root->removeAllChildren();
    QVERIFY(root->children().count() == 0);

    Qt3D::QNode *firstChild = child;
    for (int i = 0; i < 10; i++) {
        child->addChild(new MyQNode());
        child = child->children().first();
    }
    QVERIFY(root->children().count() == 0);
    root->addChild(firstChild);
    QVERIFY(root->children().count() == 1);

    Qt3D::QNode *parent = child->parentNode();
    for (int i = 0; i < 10; i++) {
        QVERIFY(parent->children().count() == 1);
        QVERIFY(child->parentNode() == parent);
        parent->removeChild(child);
        QVERIFY(child->parent() == Q_NULLPTR);
        QVERIFY(parent->children().count() == 0);
        child = parent;
        parent = parent->parentNode();
    }
    QVERIFY(parent == root);
    QVERIFY(parent->children().first() == firstChild);
    QVERIFY(firstChild->children().isEmpty());
}

void tst_Nodes::appendingChildEntitiesToNode()
{
    MyQNode *root = new MyQNode();

    Qt3D::QEntity *childEntity = new Qt3D::QEntity();
    root->addChild(childEntity);

    QVERIFY(root->children().first() == childEntity);
    QVERIFY(childEntity->parentEntity() == Q_NULLPTR);
    QVERIFY(childEntity->parentNode() == root);
}

void tst_Nodes::removingChildEntitiesFromNode()
{
    MyQNode *root = new MyQNode();

    Qt3D::QEntity *childEntity = new Qt3D::QEntity();
    root->addChild(childEntity);

    QVERIFY(root->children().first() == childEntity);
    QVERIFY(childEntity->parentEntity() == Q_NULLPTR);
    QVERIFY(childEntity->parentNode() == root);

    root->removeChild(childEntity);

    QVERIFY(root->children().isEmpty());
    QVERIFY(childEntity->parentNode() == Q_NULLPTR);
    QVERIFY(childEntity->parent() == Q_NULLPTR);
}

void tst_Nodes::appendingComponentsToEntity()
{
    MyQNode *root = new MyQNode();

    Qt3D::QEntity *entity = new Qt3D::QEntity(root);
    root->addChild(entity);

    MyQComponent *comp1 = new MyQComponent(root);
    root->addChild(comp1);

    MyQComponent *comp2 = new MyQComponent(entity);
    entity->addChild(comp2);
    MyQComponent *comp3 = new MyQComponent();

    QVERIFY(entity->parentNode() == root);
    QVERIFY(root->children().count() == 2);
    QVERIFY(entity->children().count() == 1);
    QVERIFY(entity->components().empty());
    QVERIFY(comp3->parentNode() == Q_NULLPTR);

    entity->addComponent(comp1);
    QVERIFY(entity->components().count() == 1);
    QVERIFY(entity->components().first() == comp1);
    QVERIFY(comp1->parentNode() == root);
    QVERIFY(comp2->parentNode() == entity);
    QVERIFY(comp3->parentNode() == Q_NULLPTR);

    entity->addComponent(comp2);
    QVERIFY(entity->components().count() == 2);
    QVERIFY(entity->components().first() == comp1);
    QVERIFY(entity->components().last() == comp2);
    QVERIFY(comp1->parentNode() == root);
    QVERIFY(comp2->parentNode() == entity);
    QVERIFY(comp3->parentNode() == Q_NULLPTR);

    entity->addComponent(comp3);
    QVERIFY(entity->components().count() == 3);
    QVERIFY(entity->components().first() == comp1);
    QVERIFY(entity->components().last() == comp3);
    QVERIFY(comp1->parentNode() == root);
    QVERIFY(comp2->parentNode() == entity);
    QVERIFY(comp3->parentNode() == entity);
    QCOMPARE(entity->children().count(), 2);
}

void tst_Nodes::removingComponentsFromEntity()
{
    MyQNode *root = new MyQNode();
    Qt3D::QEntity *entity = new Qt3D::QEntity(root);

    MyQComponent *comp1 = new MyQComponent(root);
    MyQComponent *comp2 = new MyQComponent(entity);
    MyQComponent *comp3 = new MyQComponent();


    entity->addComponent(comp1);
    entity->addComponent(comp2);
    entity->addComponent(comp3);
    QVERIFY(entity->components().count() == 3);
    QCOMPARE(entity->children().count(), 2);

    entity->removeComponent(comp2);
    QVERIFY(entity->components().count() == 2);
    QVERIFY(comp2->parent() == entity);
    QVERIFY(entity->children().count() == 2);

    entity->removeComponent(comp1);
    QVERIFY(entity->components().count() == 1);
    QVERIFY(comp1->parentNode() == root);

    entity->removeComponent(comp3);
    QVERIFY(entity->components().count() == 0);
    QVERIFY(comp3->parentNode() == entity);

    entity->addComponent(comp1);
    entity->addComponent(comp2);
    entity->addComponent(comp3);
    QVERIFY(entity->components().count() == 3);
    QCOMPARE(entity->children().count(), 2);

    entity->removeAllChildren();
    QCOMPARE(entity->components().count(), 3);
    QCOMPARE(entity->children().count(), 0);
}

void tst_Nodes::checkCloning()
{
    Qt3D::QScene *scene = new Qt3D::QScene();
    MyQNode *root = new MyQNode();
    root->setScene(scene);
    Qt3D::QEntity *entity = new Qt3D::QEntity(root);
    root->addChild(entity);

    MyQComponent *comp1 = new MyQComponent();
    MyQComponent *comp2 = new MyQComponent();
    MyQComponent *comp3 = new MyQComponent();

    MyQNode *childNode = new MyQNode();
    entity->addChild(childNode);
    entity->addComponent(comp1);
    entity->addComponent(comp2);
    entity->addComponent(comp3);

    root->setCustomProperty(QStringLiteral("Corvette"));

    QVERIFY(root->customProperty() == QStringLiteral("Corvette"));
    QCOMPARE(root->children().count(), 1);
    QCOMPARE(entity->children().count(), 4);
    QCOMPARE(entity->components().count(), 3);

    Qt3D::QNode *rootClone = root->clone();
    QVERIFY(rootClone->uuid() == root->uuid());
    QVERIFY(qobject_cast<MyQNode *>(rootClone) != Q_NULLPTR);
    QVERIFY(qobject_cast<MyQNode *>(rootClone)->customProperty() == root->customProperty());

    Qt3D::QEntity *entityClone = qobject_cast<Qt3D::QEntity *>(rootClone->children().first());
    QVERIFY(entity->uuid() == entityClone->uuid());
    QCOMPARE(root->children().count(), rootClone->children().count());
    QCOMPARE(entityClone->children().count(), entity->children().count());
    QCOMPARE(entityClone->components().count(), entity->components().count());
}

void tst_Nodes::checkDestruction()
{
    MyQNode *root = new MyQNode();
    Qt3D::QEntity *entity = new Qt3D::QEntity(root);

    MyQComponent *comp1 = new MyQComponent();
    MyQComponent *comp2 = new MyQComponent();
    MyQComponent *comp3 = new MyQComponent();


    entity->addComponent(comp1);
    entity->addComponent(comp2);
    entity->addComponent(comp3);

    root->addChild(entity);

    delete root;
}

QTEST_APPLESS_MAIN(tst_Nodes)

#include "tst_nodes.moc"
