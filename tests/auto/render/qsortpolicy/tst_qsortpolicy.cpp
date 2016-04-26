/****************************************************************************
**
** Copyright (C) 2015 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:GPL-EXCEPT$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtTest/QTest>
#include <Qt3DCore/private/qnode_p.h>
#include <Qt3DCore/private/qscene_p.h>
#include <Qt3DCore/qentity.h>

#include <Qt3DRender/qsortpolicy.h>

#include "testpostmanarbiter.h"

// We need to call QNode::clone which is protected
// So we sublcass QNode instead of QObject
class tst_QSortPolicy: public Qt3DCore::QNode
{
    Q_OBJECT
public:
    ~tst_QSortPolicy()
    {
        QMetaObject::invokeMethod(this, "_q_cleanup", Qt::DirectConnection);
    }

private Q_SLOTS:

    void checkSaneDefaults()
    {
        QScopedPointer<Qt3DRender::QSortPolicy> defaultsortPolicy(new Qt3DRender::QSortPolicy);

        QVERIFY(defaultsortPolicy->sortTypes().isEmpty());
    }

    void checkCloning_data()
    {
        QTest::addColumn<Qt3DRender::QSortPolicy *>("sortPolicy");
        QTest::addColumn<QVector<Qt3DRender::QSortPolicy::SortType> >("sortTypes");

        Qt3DRender::QSortPolicy *defaultConstructed = new Qt3DRender::QSortPolicy();
        QTest::newRow("defaultConstructed") << defaultConstructed << QVector<Qt3DRender::QSortPolicy::SortType>();

        Qt3DRender::QSortPolicy *sortPolicyWithSortTypes = new Qt3DRender::QSortPolicy();
        Qt3DRender::QSortPolicy::SortType sortType1 = Qt3DRender::QSortPolicy::BackToFront;
        Qt3DRender::QSortPolicy::SortType sortType2 = Qt3DRender::QSortPolicy::Material;
        QVector<Qt3DRender::QSortPolicy::SortType> sortTypes; sortTypes << sortType1 << sortType2;
        sortPolicyWithSortTypes->addSortType(sortType1);
        sortPolicyWithSortTypes->addSortType(sortType2);
        QTest::newRow("sortPolicyWithSortTypes") << sortPolicyWithSortTypes << sortTypes ;
    }

    void checkCloning()
    {
        // GIVEN
        QFETCH(Qt3DRender::QSortPolicy*, sortPolicy);
        QFETCH(QVector<Qt3DRender::QSortPolicy::SortType>, sortTypes);

        // THEN
        QCOMPARE(sortPolicy->sortTypes(), sortTypes);

        // WHEN
        Qt3DRender::QSortPolicy *clone = static_cast<Qt3DRender::QSortPolicy *>(QNode::clone(sortPolicy));

        // THEN
        QVERIFY(clone != Q_NULLPTR);
        QCOMPARE(sortPolicy->id(), clone->id());

        QCOMPARE(sortPolicy->sortTypes().count(), clone->sortTypes().count());

        for (int i = 0, m = sortTypes.count(); i < m; ++i) {
            Qt3DRender::QSortPolicy::SortType cClone = clone->sortTypes().at(i);
            Qt3DRender::QSortPolicy::SortType cOrig = sortTypes.at(i);
            QCOMPARE(cOrig,cClone);
        }

        delete sortPolicy;
        delete clone;
    }

    void checkPropertyUpdates()
    {
        QSKIP("Wait for cloning mechanism to be fixed");

        // GIVEN
        QScopedPointer<Qt3DRender::QSortPolicy> sortPolicy(new Qt3DRender::QSortPolicy());
        TestArbiter arbiter(sortPolicy.data());

        // WHEN
        Qt3DRender::QSortPolicy::SortType sortType1;
        sortPolicy->addSortType(sortType1);
        QCoreApplication::processEvents();

        // THEN
        QCOMPARE(arbiter.events.size(), 1);
        Qt3DCore::QNodePropertyChangePtr change = arbiter.events.first().staticCast<Qt3DCore::QNodePropertyChange>();
        QCOMPARE(change->propertyName(), "sortType");
        QCOMPARE(change->subjectId(),sortPolicy->id());
        QCOMPARE(change->value().value<Qt3DRender::QSortPolicy::SortType>(), sortType1);
        QCOMPARE(change->type(), Qt3DCore::NodeAdded);

        arbiter.events.clear();

        // WHEN
        sortPolicy->addSortType(sortType1);
        QCoreApplication::processEvents();

        // THEN
        QCOMPARE(arbiter.events.size(), 0);

        // WHEN
        sortPolicy->removeSortType(sortType1);
        QCoreApplication::processEvents();

        // THEN
        QCOMPARE(arbiter.events.size(), 1);
        change = arbiter.events.first().staticCast<Qt3DCore::QNodePropertyChange>();
        QCOMPARE(change->propertyName(), "sortType");
        QCOMPARE(change->subjectId(), sortPolicy->id());
        QCOMPARE(change->value().value<Qt3DRender::QSortPolicy::SortType>(), sortType1);
        QCOMPARE(change->type(), Qt3DCore::NodeRemoved);

        arbiter.events.clear();
    }

protected:
    Qt3DCore::QNode *doClone() const Q_DECL_OVERRIDE
    {
        return Q_NULLPTR;
    }

};

QTEST_MAIN(tst_QSortPolicy)

#include "tst_qsortpolicy.moc"
