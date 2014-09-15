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

#include "qtechnique.h"
#include "qtechnique_p.h"
#include "qparameter.h"
#include "qopenglfilter.h"
#include <Qt3DCore/qscenepropertychange.h>
#include <QDebug>

QT_BEGIN_NAMESPACE

namespace Qt3D {

QTechniquePrivate::QTechniquePrivate(QTechnique *qq)
    : QAbstractTechniquePrivate(qq)
    , m_openGLFilter(new QOpenGLFilter())
{
}

QTechniquePrivate::~QTechniquePrivate()
{
    delete m_openGLFilter;
}

QTechnique::QTechnique(QNode *parent)
    : QAbstractTechnique(*new QTechniquePrivate(this), parent)
{
    Q_D(QTechnique);
    QObject::connect(d->m_openGLFilter, SIGNAL(openGLFilterChanged()), this, SLOT(openGLFilterChanged()));
}

void QTechnique::copy(const QNode *ref)
{
    Q_D(QTechnique);
    QAbstractTechnique::copy(ref);
    const QTechnique *tech = qobject_cast<const QTechnique *>(ref);
    if (tech != Q_NULLPTR) {
        d->m_openGLFilter->copy(tech->openGLFilter());
    }
}

QTechnique::QTechnique(QTechniquePrivate &dd, QNode *parent)
    : QAbstractTechnique(dd, parent)
{
    Q_D(QTechnique);
    QObject::connect(d->m_openGLFilter, SIGNAL(openGLFilterChanged()), this, SLOT(openGLFilterChanged()));
}

QTechnique *QTechnique::doClone(QNode *clonedParent) const
{
    Q_D(const QTechnique);
    QTechnique *technique = new QTechnique(clonedParent);

    Q_FOREACH (QCriterion *criterion, d->m_criteriaList)
        technique->addCriterion(qobject_cast<QCriterion *>(criterion->clone(technique)));
    Q_FOREACH (QAbstractRenderPass *pass, d->m_renderPasses)
        technique->addPass(qobject_cast<QAbstractRenderPass *>(pass->clone(technique)));
    Q_FOREACH (QParameter *p, d->m_parameters)
        technique->addParameter(qobject_cast<QParameter *>(p->clone(technique)));

    return technique;
}

void QTechnique::openGLFilterChanged()
{
    Q_D(QTechnique);
    if (d->m_changeArbiter != Q_NULLPTR) {
        QScenePropertyChangePtr change(new QScenePropertyChange(NodeUpdated, this));
        change->setPropertyName(QByteArrayLiteral("openGLFilter"));
        QOpenGLFilter *clone = new QOpenGLFilter();
        clone->copy(d->m_openGLFilter);
        change->setValue(QVariant::fromValue(clone));
        notifyObservers(change);
    }
}

void QTechnique::addCriterion(QCriterion *criterion)
{
    Q_D(QTechnique);
    if (!d->m_criteriaList.contains(criterion)) {
        d->m_criteriaList.append(criterion);

        // We need to add it as a child of the current node if it has been declared inline
        // Or not previously added as a child of the current node so that
        // 1) The backend gets notified about it's creation
        // 2) When the current node is destroyed, it gets destroyed as well
        if (!criterion->parent() || criterion->parent() == this)
            QNode::addChild(criterion);

        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr change(new QScenePropertyChange(NodeAdded, this));
            change->setPropertyName(QByteArrayLiteral("criterion"));
            change->setValue(QVariant::fromValue(criterion->clone()));
            notifyObservers(change);
        }
    }
}

void QTechnique::removeCriterion(QCriterion *criterion)
{
    Q_D(QTechnique);
    if (d->m_changeArbiter != Q_NULLPTR) {
        QScenePropertyChangePtr change(new QScenePropertyChange(NodeRemoved, this));
        change->setPropertyName(QByteArrayLiteral("criterion"));
        change->setValue(QVariant::fromValue(criterion->uuid()));
        notifyObservers(change);
    }
    d->m_criteriaList.removeOne(criterion);
}

QList<QCriterion *> QTechnique::criteria() const
{
    Q_D(const QTechnique);
    return d->m_criteriaList;
}

void QTechnique::clearCriteria()
{
    Q_D(QTechnique);
    while (d->m_criteriaList.size() > 0)
        removeCriterion(d->m_criteriaList.takeFirst());
}

void QTechnique::addParameter(QParameter *parameter)
{
    Q_D(QTechnique);
    if (!d->m_parameters.contains(parameter)) {
        d->m_parameters.append(parameter);

        // We need to add it as a child of the current node if it has been declared inline
        // Or not previously added as a child of the current node so that
        // 1) The backend gets notified about it's creation
        // 2) When the current node is destroyed, the child parameters get destroyed as well
        if (!parameter->parent() || parameter->parent() == this)
            QNode::addChild(parameter);

        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr change(new QScenePropertyChange(NodeAdded, this));
            change->setPropertyName(QByteArrayLiteral("parameter"));
            change->setValue(QVariant::fromValue(parameter->clone()));
            notifyObservers(change);
        }
    }
}

void QTechnique::removeParameter(QParameter *parameter)
{
    Q_D(QTechnique);

    if (d->m_changeArbiter != Q_NULLPTR) {
        QScenePropertyChangePtr change(new QScenePropertyChange(NodeRemoved, this));
        change->setPropertyName(QByteArrayLiteral("parameter"));
        change->setValue(QVariant::fromValue(parameter->clone()));
        notifyObservers(change);
    }
    d->m_parameters.removeOne(parameter);
}

QList<QParameter *> QTechnique::parameters() const
{
    Q_D(const QTechnique);
    return d->m_parameters;
}

QParameter *QTechnique::parameterByName(QString name) const
{
    Q_D(const QTechnique);
    foreach (QParameter* p, d->m_parameters) {
        if (p->name() == name)
            return p;
    }
    return NULL;
}

QOpenGLFilter *QTechnique::openGLFilter() const
{
    Q_D(const QTechnique);
    return d->m_openGLFilter;
}

} // of namespace Qt3D

QT_END_NAMESPACE
