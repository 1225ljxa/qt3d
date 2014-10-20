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

#include "qsortmethod.h"
#include "qsortmethod_p.h"
#include "qsortcriterion_p.h"
#include <Qt3DCore/qscenepropertychange.h>
#include <Qt3DRenderer/qsortcriterion.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

QSortMethodPrivate::QSortMethodPrivate(QSortMethod *qq)
    : QFrameGraphNodePrivate(qq)
{
}

void QSortMethod::copy(const QNode *ref)
{
    QFrameGraphNode::copy(ref);
    const QSortMethod *other = static_cast<const QSortMethod*>(ref);
    Q_FOREACH (QSortCriterion *c, other->d_func()->m_criteria)
        addCriterion(qobject_cast<QSortCriterion *>(QNodePrivate::get(c)->clone()));
}

QSortMethod::QSortMethod(QNode *parent)
    : QFrameGraphNode(*new QSortMethodPrivate(this), parent)
{
}

QSortMethod::QSortMethod(QSortMethodPrivate &dd, QNode *parent)
    : QFrameGraphNode(dd, parent)
{
}

void QSortMethod::addCriterion(QSortCriterion *criterion)
{
    Q_D(QSortMethod);
    if (!d->m_criteria.contains(criterion)) {
        d->m_criteria.append(criterion);

        if (!criterion->parent())
            criterion->setParent(this);

        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr propertyChange(new QScenePropertyChange(NodeAdded, this));
            propertyChange->setPropertyName(QByteArrayLiteral("sortCriterion"));
            propertyChange->setValue(QVariant::fromValue(criterion));
            d->notifyObservers(propertyChange);
        }
    }
}

void QSortMethod::removeCriterion(QSortCriterion *criterion)
{
    Q_D(QSortMethod);
    if (d->m_changeArbiter != Q_NULLPTR) {
        QScenePropertyChangePtr propertyChange(new QScenePropertyChange(NodeRemoved, this));
        propertyChange->setPropertyName(QByteArrayLiteral("sortCriterion"));
        propertyChange->setValue(QVariant::fromValue(criterion));
        d->notifyObservers(propertyChange);
    }
    d->m_criteria.removeOne(criterion);
}

QList<QSortCriterion *> QSortMethod::criteria() const
{
    Q_D(const QSortMethod);
    return d->m_criteria;
}

} // Qt3D

QT_END_NAMESPACE
