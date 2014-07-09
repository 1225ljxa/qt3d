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
#include "renderlogging.h"
#include <Qt3DCore/qscenepropertychange.h>
#include <private/qparameter_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {


QParameterPrivate::QParameterPrivate(QParameter *qq)
    : QNodePrivate(qq)
    , m_type(QParameter::Undefined)
{
}

QParameter::QParameter(QParameterPrivate &dd, QNode *parent)
    : QNode(dd, parent)

{
}

QParameter::QParameter(QNode *parent)
    : QNode(*new QParameterPrivate(this), parent)
{
}

QParameter::QParameter(QNode *parent, const QString &name, const QVariant &value, QParameter::OpenGLTypes ty)
    : QNode(*new QParameterPrivate(this), parent)
{
    Q_D(QParameter);
    d->m_name = name;
    d->m_value = value;
    d->m_type = ty;
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
        d->m_value = dv;
        emit valueChanged();
        QScenePropertyChangePtr change(new QScenePropertyChange(ComponentUpdated, this));
        change->setPropertyName(d->m_name.toUtf8());
        change->setValue(d->m_value);
        notifyObservers(change);
    }
}

QVariant QParameter::value() const
{
    Q_D(const QParameter);
    return d->m_value;
}

QParameter::OpenGLTypes QParameter::datatype() const
{
    Q_D(const QParameter);
    return d->m_type;
}

void QParameter::setDatatype(OpenGLTypes type)
{
    Q_D(QParameter);
    if (d->m_type != type) {
        d->m_type = type;
        emit datatypeChanged();
    }
}

bool QParameter::isTextureType() const
{
    Q_D(const QParameter);
    switch (d->m_type) {
    case Sampler1D:
    case Sampler2D:
    case Sampler3D:
    case SamplerCube:
        return true;
    default:
        return false;
    }
}

Render::QUniformValue::Type QParameter::uniformType() const
{
    Q_D(const QParameter);
    switch (d->m_type) {
    case Bool:
    case BoolVec2:
    case BoolVec3:
    case BoolVec4:
        return Render::QUniformValue::Bool;

    // integers!

    case Float:
    case FloatVec2:
    case FloatVec3:
    case FloatVec4:
    case FloatMat3:
    case FloatMat4:
        return Render::QUniformValue::Float;

    case Double:
    case DoubleVec2:
    case DoubleVec3:
    case DoubleVec4:
        return Render::QUniformValue::Double;

    default:
        qCWarning(Render::Backend) << Q_FUNC_INFO << "couldn't map datatype:" << QString::number(d->m_type, 16);
        return Render::QUniformValue::Invalid;
    }
}

} // Qt3D

QT_END_NAMESPACE
