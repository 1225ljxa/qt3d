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

#include "qabstracttechnique.h"
#include "qabstractrenderpass.h"

/*!
 * \class QAbstractTechnique
 * \namespace Qt3D
 *
 * \inherits Node
 *
 * \brief Provides an abstract class that should be the base of all
 * Technique classes in a scene.
 *
 * A QAbstractTechnique is a container of QAbstractRenderPasses. If it contains
 * several passes, that doesn't mean they will all be used during rendering. This
 * depends on the aspect behind used for that purpose and any pass filtering set up
 * by the QAbstractTechnique subclass.
 *
 * \sa QAbstractEffect, QAbstractRenderPass
 */

QT_BEGIN_NAMESPACE

namespace Qt3D {

QAbstractTechnique::QAbstractTechnique(Node *parent)
    : Node(parent)
    , QObservable()
    , d_ptr(new QAbstractTechniquePrivate(this))
{
}

/*!
 * Sets the \a name of the technique
 */
void QAbstractTechnique::setName(const QString &name)
{
    Q_D(QAbstractTechnique);
    if (d->m_name != name) {
        d->m_name = name;
        emit nameChanged();
    }
}

/*!
 * Returns the name of the technique
 */
QString QAbstractTechnique::name() const
{
    Q_D(const QAbstractTechnique);
    return d->m_name;
}

/*!
 * Appends a \a pass to the technique. This posts a ComponentAdded
 * QScenePropertyChange notification to the QChangeArbiter with the
 * value being the \a pass and the property name being "pass".
 */
void QAbstractTechnique::addPass(QAbstractRenderPass *pass)
{
    Q_D(QAbstractTechnique);
    if (!d->m_renderPasses.contains(pass)) {
        d->m_renderPasses.append(pass);
        QScenePropertyChangePtr e(new QScenePropertyChange(ComponentAdded, this));
        e->m_propertyName = QByteArrayLiteral("pass");
        e->m_value = QVariant::fromValue(pass);
        notifyObservers(e);
    }
}

/*!
 * Removes a \a pass from the technique. This posts a ComponentRemoved
 * QScenePropertyChange notification to the QChangeArbiter with the value
 * being the \a pass and the property name being "pass".
 */
void QAbstractTechnique::removePass(QAbstractRenderPass *pass)
{
    Q_D(QAbstractTechnique);
    d->m_renderPasses.removeOne(pass);
    QScenePropertyChangePtr e(new QScenePropertyChange(ComponentRemoved, this));
    e->m_propertyName = QByteArrayLiteral("pass");
    e->m_value = QVariant::fromValue(pass);
    notifyObservers(e);
}

/*!
 * Returns the list of render passes contained in the technique.
 */
QList<QAbstractRenderPass *> QAbstractTechnique::renderPasses() const
{
    Q_D(const QAbstractTechnique);
    return d->m_renderPasses;
}

} // Qt3D

QT_END_NAMESPACE
