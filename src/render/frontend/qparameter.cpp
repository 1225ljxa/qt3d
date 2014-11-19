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

#include "qparameter.h"
#include <Qt3DRenderer/private/renderlogging_p.h>
#include <Qt3DCore/qscenepropertychange.h>
#include <private/qparameter_p.h>
#include <Qt3DRenderer/qtexture.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {


QParameterPrivate::QParameterPrivate(QParameter *qq)
    : QNodePrivate(qq)
    , m_isTexture(false)
{
}

void QParameterPrivate::setValue(const QVariant &v)
{
    m_value = v;
}

void QParameter::copy(const QNode *ref)
{
    QNode::copy(ref);
    const QParameter *param = static_cast<const QParameter*>(ref);
    d_func()->m_name = param->d_func()->m_name;
    d_func()->m_value = param->d_func()->m_value;
    d_func()->m_isTexture = param->d_func()->m_isTexture;
}

QParameter::QParameter(QParameterPrivate &dd, QNode *parent)
    : QNode(dd, parent)
{
}

QParameter::QParameter(QNode *parent)
    : QNode(*new QParameterPrivate(this), parent)
{
}

QParameter::QParameter(const QString &name, const QVariant &value, QNode *parent)
    : QNode(*new QParameterPrivate(this), parent)
{
    Q_D(QParameter);
    d->m_name = name;
    setValue(value);
}

QParameter::QParameter(const QString &name, QTexture *texture, QNode *parent)
    : QNode(*new QParameterPrivate(this), parent)
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
        emit nameChanged();
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
        emit valueChanged();

        // In case texture are declared inline
        QTexture *txt = dv.value<QTexture *>();
        if ((d->m_isTexture = (txt != Q_NULLPTR)) && !txt->parent())
           txt->setParent(this);

        QScenePropertyChangePtr change(new QScenePropertyChange(NodeUpdated, this));
        change->setPropertyName(d->m_name.toUtf8());
        change->setValue(d->m_value);
        d->notifyObservers(change);
    }
}

QVariant QParameter::value() const
{
    Q_D(const QParameter);
    return d->m_value;
}


bool QParameter::isTextureType() const
{
    Q_D(const QParameter);
    return d->m_isTexture;
}

} // Qt3D

QT_END_NAMESPACE
