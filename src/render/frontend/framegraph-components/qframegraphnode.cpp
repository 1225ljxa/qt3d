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

#include "qframegraphnode.h"
#include "qframegraphnode_p.h"

/*!
 * \class FrameGraphNode
 *
 * \brief Base class of all FrameGraph configuration nodes.
 *
 * This is an abstract class so it cannot be instanced directly
 * but rather through one of its subclasses.
 *
 * \since 5.3
 * \namespace Qt3D
 */

QT_BEGIN_NAMESPACE

namespace Qt3D {

QFrameGraphNodePrivate::QFrameGraphNodePrivate(QFrameGraphNode *qq)
    : QNodePrivate(qq)
    , m_enabled(true)
{
}

void QFrameGraphNode::copy(const QNode *ref)
{
    QNode::copy(ref);
}

QFrameGraphNode::QFrameGraphNode(QNode *parent)
    : QNode(*new QFrameGraphNodePrivate(this), parent)
{
}

QFrameGraphNode::QFrameGraphNode(QFrameGraphNodePrivate &dd, QNode *parent)
    : QNode(dd, parent)
{
}

void QFrameGraphNode::appendFrameGraphNode(QFrameGraphNode *item)
{
    Q_D(QFrameGraphNode);
    if (!d->m_fgChildren.contains(item)) {
        if (!item->parent())
            item->setParent(this);
        d->m_fgChildren.append(item);
    }
}

void QFrameGraphNode::removeFrameGraphNode(QFrameGraphNode *item)
{
    Q_D(QFrameGraphNode);
    if (!d->m_fgChildren.contains(item)) {
        d->m_fgChildren.removeOne(item);
    }
}

QList<QFrameGraphNode *> QFrameGraphNode::frameGraphChildren() const
{
    Q_D(const QFrameGraphNode);
    return d->m_fgChildren;
}


} // Qt3D

QT_END_NAMESPACE
