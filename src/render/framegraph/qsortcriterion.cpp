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

#include "qsortcriterion.h"
#include "qsortcriterion_p.h"
#include <Qt3DCore/qscenepropertychange.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

/*!
    \class Qt3DRender::QSortCriterionPrivate
    \internal
*/
QSortCriterionPrivate::QSortCriterionPrivate()
    : QNodePrivate()
    , m_sort(QSortCriterion::StateChangeCost)
{
}

void QSortCriterion::copy(const QNode *ref)
{
    QNode::copy(ref);
    const QSortCriterion *s = static_cast<const QSortCriterion*>(ref);
    d_func()->m_sort = s->d_func()->m_sort;
}

QSortCriterion::QSortCriterion(QNode *parent)
    : QNode(*new QSortCriterionPrivate, parent)
{
}

QSortCriterion::~QSortCriterion()
{
    QNode::cleanup();
}

QSortCriterion::SortType QSortCriterion::sort() const
{
    Q_D(const QSortCriterion);
    return d->m_sort;
}

void QSortCriterion::setSort(QSortCriterion::SortType sort)
{
    Q_D(QSortCriterion);
    if (d->m_sort != sort) {
        d->m_sort = sort;
        emit sortChanged(sort);
    }
}

/*! \internal */
QSortCriterion::QSortCriterion(QSortCriterionPrivate &dd, QNode *parent)
    : QNode(dd, parent)
{
}

} // namespace Qt3DRender

QT_END_NAMESPACE
