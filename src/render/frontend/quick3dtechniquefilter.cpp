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

#include "quick3dtechniquefilter.h"

QT_BEGIN_NAMESPACE

namespace Qt3D {

namespace Quick {

Quick3DTechniqueFilter::Quick3DTechniqueFilter(Node *parent)
    : TechniqueFilter()
    , Quick3DFrameGraphItem(parent)
{
}

QQmlListProperty<Qt3D::Tag> Quick3DTechniqueFilter::tagList()
{
    return QQmlListProperty<Qt3D::Tag>(this, 0,
                                       &Quick3DTechniqueFilter::appendTag,
                                       &Quick3DTechniqueFilter::tagCount,
                                       &Quick3DTechniqueFilter::tagAt,
                                       &Quick3DTechniqueFilter::clearTags);
}

void Quick3DTechniqueFilter::appendTag(QQmlListProperty<Tag> *list, Tag *tag)
{
    Quick3DTechniqueFilter *filter = qobject_cast<Quick3DTechniqueFilter *>(list->object);
    if (filter) {
        tag->setParent(filter);
        filter->addTag(tag);
    }
}

Tag *Quick3DTechniqueFilter::tagAt(QQmlListProperty<Tag> *list, int index)
{
    TechniqueFilter *filter = qobject_cast<TechniqueFilter *>(list->object);
    if (filter)
        return filter->tags().at(index);
    return 0;
}

int Quick3DTechniqueFilter::tagCount(QQmlListProperty<Tag> *list)
{
    TechniqueFilter *filter = qobject_cast<TechniqueFilter *>(list->object);
    if (filter)
        return filter->tags().size();
    return 0;
}

void Quick3DTechniqueFilter::clearTags(QQmlListProperty<Tag> *list)
{
    TechniqueFilter *filter = qobject_cast<TechniqueFilter *>(list->object);
    if (filter) {
        Q_FOREACH (Tag *tag, filter->tags())
            filter->removeTag(tag);
    }
}

} // Quick

} // Qt3D

QT_END_NAMESPACE
