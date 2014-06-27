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

#include "effect.h"
#include "technique.h"
#include "parameter.h"

#include <Qt3DCore/qscenepropertychange.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

class EffectPrivate
{
public :
    EffectPrivate(Effect *qq)
        : q_ptr(qq)
    {}

    QList<Parameter *> m_parameters;
    Q_DECLARE_PUBLIC(Effect)
    Effect *q_ptr;
};

Effect::Effect(Node *parent)
    : QAbstractEffect(parent)
    , d_ptr(new EffectPrivate(this))
{
}

void Effect::addTechnique(QAbstractTechnique *t)
{
    // In the C++ API we are responsible for setting the parent
    // Qml API is automatically handled by the Qml Engine
    if (!t->parent())
        t->setParent(this);
    QAbstractEffect::addTechnique(t);
}

void Effect::removeTechnique(QAbstractTechnique *t)
{
    QAbstractEffect::removeTechnique(t);
}

void Effect::addParameter(Parameter *parameter)
{
    Q_D(Effect);
    if (!d->m_parameters.contains(parameter)) {
        d->m_parameters.append(parameter);
        QScenePropertyChangePtr change(new QScenePropertyChange(ComponentAdded, this));
        change->m_propertyName = QByteArrayLiteral("parameter");
        change->m_value = QVariant::fromValue(parameter);
        notifyObservers(change);
    }
}

void Effect::removeParameter(Parameter *parameter)
{
    Q_D(Effect);
    d->m_parameters.removeOne(parameter);
    QScenePropertyChangePtr change(new QScenePropertyChange(ComponentRemoved, this));
    change->m_propertyName = QByteArrayLiteral("parameter");
    change->m_value = QVariant::fromValue(parameter);
    notifyObservers(change);
}

QList<Parameter *> Effect::parameters() const
{
    Q_D(const Effect);
    return d->m_parameters;
}

} // Qt3D

QT_END_NAMESPACE
