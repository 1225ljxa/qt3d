/****************************************************************************
**
** Copyright (C) 2015 Klaralvdalens Datakonsult AB (KDAB).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
****************************************************************************/

#include "qactioninput.h"
#include <Qt3DCore/private/qnode_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3DInput {

class QActionInputPrivate : public Qt3DCore::QNodePrivate
{
public:
    QActionInputPrivate()
        : Qt3DCore::QNodePrivate()
        , m_sourceDevice(Q_NULLPTR)
    {}

    QVector<int> m_keys;
    QAbstractPhysicalDevice *m_sourceDevice;
};

QActionInput::QActionInput(Qt3DCore::QNode *parent)
    : Qt3DCore::QNode(*new QActionInputPrivate(), parent)
{
}

QActionInput::~QActionInput()
{
    QNode::cleanup();
}

void QActionInput::addKey(int key)
{
    Q_D(QActionInput);
    if (!d->m_keys.contains(key))
        d->m_keys.push_back(key);
    // TO DO: add notifications
}

void QActionInput::removeKey(int key)
{
    Q_D(QActionInput);
    d->m_keys.removeOne(key);
    // TO DO: add notifications
}

QVector<int> QActionInput::keys() const
{
    Q_D(const QActionInput);
    return d->m_keys;
}

QBitArray QActionInput::keysBitArray() const
{
    // TO DO: complete
    return QBitArray();
}

void QActionInput::setSourceDevice(QAbstractPhysicalDevice *sourceDevice)
{
    Q_D(QActionInput);
    if (d->m_sourceDevice != sourceDevice) {

        // TO DO: check and set parent if needed once QAbstactInputDevice is a QNode subclass

        d->m_sourceDevice = sourceDevice;
        emit sourceDeviceChanged();
    }
}

QAbstractPhysicalDevice *QActionInput::sourceDevice() const
{
    Q_D(const QActionInput);
    return d->m_sourceDevice;
}

void QActionInput::copy(const Qt3DCore::QNode *ref)
{
    QNode::copy(ref);
    const QActionInput *input = static_cast<const QActionInput *>(ref);
    // TO DO: Convert QAbstractPhysicalDevice to a QNode
    // d_func()->m_sourceDevice = qobject_cast<QAbstractPhysicalDevice *>(QNode::clone(input->d_func()->m_sourceDevice));
    d_func()->m_keys = input->d_func()->m_keys;

}

} // Qt3DInput

QT_END_NAMESPACE
