/****************************************************************************
**
** Copyright (C) 2015 Klaralvdalens Datakonsult AB (KDAB).
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

#include "qaspectfactory.h"
#include "qaspectfactory_p.h"

#include <QDebug>

QT_BEGIN_NAMESPACE

namespace Qt3D {

QHash<QString, QAspectFactory::CreateFunction> QAspectFactoryPrivate::m_defaultFactories;

void _qt3d_QAspectFactoryPrivate_addDefaultFactory(const QString &name,
                                                   QAspectFactory::CreateFunction factory)
{
    QAspectFactoryPrivate::m_defaultFactories.insert(name, factory);
}

QAspectFactoryPrivate::QAspectFactoryPrivate()
    : m_factories(m_defaultFactories)
{
}

QAspectFactory::QAspectFactory()
    : d(new QAspectFactoryPrivate)
{
}

QAspectFactory::QAspectFactory(const QAspectFactory &other)
    : d(other.d)
{
}

QAspectFactory::~QAspectFactory()
{
}

QAspectFactory &QAspectFactory::operator=(const QAspectFactory &other)
{
    d = other.d;
    return *this;
}

void QAspectFactory::addFactory(const QString &name, QAspectFactory::CreateFunction factory)
{
    d->m_factories.insert(name, factory);
}

QStringList QAspectFactory::availableFactories() const
{
    return d->m_factories.keys();
}

QAbstractAspect *QAspectFactory::createAspect(const QString &aspect, QObject *parent) const
{
    if (d->m_factories.contains(aspect)) {
        return d->m_factories.value(aspect)(parent);
    } else {
        qWarning() << "Unsupported aspect name:" << aspect << "please check registrations";
        return Q_NULLPTR;
    }
}

} // namespace Qt3D

QT_END_NAMESPACE
