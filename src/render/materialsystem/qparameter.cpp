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

#include "qparameter.h"
#include "qparameter_p.h"
#include <Qt3DRender/private/renderlogging_p.h>
#include <Qt3DCore/qscenepropertychange.h>
#include <Qt3DRender/qtexture.h>

QT_BEGIN_NAMESPACE

using namespace Qt3DCore;

namespace Qt3DRender {

QParameterPrivate::QParameterPrivate()
    : QNodePrivate()
{
}

void QParameterPrivate::setValue(const QVariant &v)
{
    Qt3DCore::QNode *nodeValue = v.value<Qt3DCore::QNode *>();
    if (nodeValue != Q_NULLPTR)
        m_backendValue = QVariant::fromValue(nodeValue->id());
    else
        m_backendValue = v;
    m_value = v;
}

void QParameter::copy(const QNode *ref)
{
    QNode::copy(ref);
    const QParameter *param = static_cast<const QParameter*>(ref);
    d_func()->m_name = param->d_func()->m_name;
    d_func()->m_value = param->d_func()->m_value;
    d_func()->m_backendValue = param->d_func()->m_backendValue;
}

/*! \internal */
QParameter::QParameter(QParameterPrivate &dd, QNode *parent)
    : QNode(dd, parent)
{
}

QParameter::QParameter(QNode *parent)
    : QNode(*new QParameterPrivate, parent)
{
}

QParameter::~QParameter()
{
    QNode::cleanup();
}

QParameter::QParameter(const QString &name, const QVariant &value, QNode *parent)
    : QNode(*new QParameterPrivate, parent)
{
    Q_D(QParameter);
    d->m_name = name;
    setValue(value);
}

QParameter::QParameter(const QString &name, QAbstractTextureProvider *texture, QNode *parent)
    : QNode(*new QParameterPrivate, parent)
{
    Q_D(QParameter);
    d->m_name = name;
    setValue(QVariant::fromValue(texture));
}


void QParameter::setName(const QString &name)
{
    Q_D(QParameter);
    if (d->m_name != name) {
        d->m_name = name;
        emit nameChanged(name);
    }
}

QString QParameter::name() const
{
    Q_D(const QParameter);
    return d->m_name;
}

void QParameter::setValue(const QVariant &dv)
{
    Q_D(QParameter);
    if (d->m_value != dv) {
        d->setValue(dv);
        emit valueChanged(dv);

        // In case node values are declared inline
        QNode *nodeValue = dv.value<QNode *>();
        if (nodeValue != Q_NULLPTR && !nodeValue->parent())
            nodeValue->setParent(this);

        QScenePropertyChangePtr change(new QScenePropertyChange(NodeUpdated, QSceneChange::Node, id()));
        change->setPropertyName(d->m_name.toUtf8().data());
        change->setValue(d->m_backendValue);

        d->notifyObservers(change);
    }
}

QVariant QParameter::value() const
{
    Q_D(const QParameter);
    return d->m_value;
}

Qt3DCore::QNodeCreatedChangeBasePtr QParameter::createNodeCreationChange() const
{
    auto creationChange = Qt3DCore::QNodeCreatedChangePtr<QParameterData>::create(this);
    auto &data = creationChange->data;
    Q_D(const QParameter);
    data.name = d->m_name;
    data.backendValue = d->m_backendValue;
    return creationChange;
}

} // namespace Qt3DRender

QT_END_NAMESPACE
