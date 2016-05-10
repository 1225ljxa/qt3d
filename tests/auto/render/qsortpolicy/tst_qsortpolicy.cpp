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
#include <Qt3DCore/qpropertyvalueaddedchange.h>
#include <Qt3DCore/qpropertyvalueremovedchange.h>
#include <Qt3DCore/private/qnodecreatedchangegenerator_p.h>

#include <Qt3DRender/qsortpolicy.h>
#include <Qt3DRender/private/qsortpolicy_p.h>

#include "testpostmanarbiter.h"

// We need to call QNode::clone which is protected
// So we sublcass QNode instead of QObject
class tst_QSortPolicy: public Qt3DCore::QNode
{
    Q_OBJECT

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
        Qt3DCore::QNodeCreatedChangeGenerator creationChangeGenerator(sortPolicy);
        QVector<Qt3DCore::QNodeCreatedChangeBasePtr> creationChanges = creationChangeGenerator.creationChanges();

        // THEN
        QCOMPARE(creationChanges.size(), 1);

        const Qt3DCore::QNodeCreatedChangePtr<Qt3DRender::QSortPolicyData> creationChangeData =
                qSharedPointerCast<Qt3DCore::QNodeCreatedChange<Qt3DRender::QSortPolicyData>>(creationChanges.first());
        const Qt3DRender::QSortPolicyData &cloneData = creationChangeData->data;

        QCOMPARE(sortPolicy->id(), creationChangeData->subjectId());
        QCOMPARE(sortPolicy->isEnabled(), creationChangeData->isNodeEnabled());
        QCOMPARE(sortPolicy->metaObject(), creationChangeData->metaObject());
        QCOMPARE(sortPolicy->sortTypes().count(), cloneData.sortTypes.count());
        QCOMPARE(sortPolicy->sortTypes(), cloneData.sortTypes);

        delete sortPolicy;
    }

    void checkPropertyUpdates()
    {
        // GIVEN
        QScopedPointer<Qt3DRender::QSortPolicy> sortPolicy(new Qt3DRender::QSortPolicy());
        TestArbiter arbiter(sortPolicy.data());

        // WHEN
        Qt3DRender::QSortPolicy::SortType sortType1 = Qt3DRender::QSortPolicy::BackToFront;
        sortPolicy->addSortType(sortType1);
        QCoreApplication::processEvents();

        // THEN
        QCOMPARE(arbiter.events.size(), 1);
        Qt3DCore::QPropertyValueAddedChangePtr addChange = arbiter.events.first().staticCast<Qt3DCore::QPropertyValueAddedChange>();
        QCOMPARE(addChange->propertyName(), "sortType");
        QCOMPARE(addChange->subjectId(),sortPolicy->id());
        QCOMPARE(addChange->addedValue().value<Qt3DRender::QSortPolicy::SortType>(), sortType1);
        QCOMPARE(addChange->type(), Qt3DCore::PropertyValueAdded);

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
        Qt3DCore::QPropertyValueRemovedChangePtr removeChange = arbiter.events.first().staticCast<Qt3DCore::QPropertyValueRemovedChange>();
        QCOMPARE(removeChange->propertyName(), "sortType");
        QCOMPARE(removeChange->subjectId(), sortPolicy->id());
        QCOMPARE(removeChange->removedValue().value<Qt3DRender::QSortPolicy::SortType>(), sortType1);
        QCOMPARE(removeChange->type(), Qt3DCore::PropertyValueRemoved);

        arbiter.events.clear();
    }
};

QTEST_MAIN(tst_QSortPolicy)

#include "tst_qsortpolicy.moc"
