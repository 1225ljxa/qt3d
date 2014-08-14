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

#include "qscissortest.h"
#include <private/qnode_p.h>
#include <Qt3DCore/qscenepropertychange.h>


QT_BEGIN_NAMESPACE

namespace Qt3D {

class QScissorTestPrivate : public QNodePrivate
{
public:
    QScissorTestPrivate(QScissorTest *qq)
        : QNodePrivate(qq)
        , m_left(0)
        , m_bottom(0)
        , m_width(0)
        , m_height(0)
    {
    }

    Q_DECLARE_PUBLIC(QScissorTest)
    int m_left;
    int m_bottom;
    int m_width;
    int m_height;
};

QScissorTest::QScissorTest(QNode *parent)
    : QDrawState(*new QScissorTestPrivate(this), parent)
{
}

void QScissorTest::copy(const QNode *ref)
{
    QDrawState::copy(ref);
    Q_D(QScissorTest);
    const QScissorTest *refState = qobject_cast<const QScissorTest *>(ref);
    if (refState != Q_NULLPTR) {
        d->m_left = refState->left();
        d->m_bottom = refState->bottom();
        d->m_width = refState->width();
        d->m_height = refState->height();
    }
}

int QScissorTest::left() const
{
    Q_D(const QScissorTest);
    return d->m_left;
}

void QScissorTest::setLeft(int left)
{
    Q_D(QScissorTest);
    if (d->m_left != left) {
        d->m_left = left;
        emit leftChanged();
        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr propertyChange(new QScenePropertyChange(NodeUpdated, this));
            propertyChange->setPropertyName(QByteArrayLiteral("left"));
            propertyChange->setValue(d->m_left);
            notifyObservers(propertyChange);
        }
    }
}

int QScissorTest::bottom() const
{
    Q_D(const QScissorTest);
    return d->m_bottom;
}

void QScissorTest::setBottom(int bottom)
{
    Q_D(QScissorTest);
    if (d->m_bottom != bottom) {
        d->m_bottom = bottom;
        emit bottomChanged();
        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr propertyChange(new QScenePropertyChange(NodeUpdated, this));
            propertyChange->setPropertyName(QByteArrayLiteral("bottom"));
            propertyChange->setValue(d->m_bottom);
            notifyObservers(propertyChange);
        }
    }
}

int QScissorTest::width() const
{
    Q_D(const QScissorTest);
    return d->m_width;
}

void QScissorTest::setWidth(int width)
{
    Q_D(QScissorTest);
    if (d->m_width != width) {
        d->m_width = width;
        emit widthChanged();
        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr propertyChange(new QScenePropertyChange(NodeUpdated, this));
            propertyChange->setPropertyName(QByteArrayLiteral("width"));
            propertyChange->setValue(d->m_width);
            notifyObservers(propertyChange);
        }
    }
}

int QScissorTest::height() const
{
    Q_D(const QScissorTest);
    return d->m_height;
}

void QScissorTest::setHeight(int height)
{
    Q_D(QScissorTest);
    if (d->m_height != height) {
        d->m_height = height;
        emit heightChanged();
        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr propertyChange(new QScenePropertyChange(NodeUpdated, this));
            propertyChange->setPropertyName(QByteArrayLiteral("height"));
            propertyChange->setValue(d->m_height);
            notifyObservers(propertyChange);
        }
    }
}

QNode *QScissorTest::doClone(QNode *clonedParent) const
{
    return new QScissorTest(clonedParent);
}

} // Qt3D

QT_END_NAMESPACE
