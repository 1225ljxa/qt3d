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

#include "qrenderpass.h"
#include "qrenderpass_p.h"
#include "qparameter.h"
#include "renderpasscriterion.h"
#include "qparametermapper.h"

QT_BEGIN_NAMESPACE

namespace Qt3D {

QRenderPassPrivate::QRenderPassPrivate(QRenderPass *qq)
    : QAbstractRenderPassPrivate(qq)
    , m_stateSet(0)
{
}

QRenderPass::QRenderPass(QNode *parent)
    : QAbstractRenderPass(*new QRenderPassPrivate(this), parent)
{
}

QRenderPass::QRenderPass(QRenderPassPrivate &dd, QNode *parent)
    : QAbstractRenderPass(dd, parent)
{
}

ParameterList QRenderPass::attributes() const
{
    Q_D(const QRenderPass);
    return d->m_attributes;
}

ParameterList QRenderPass::uniforms() const
{
    Q_D(const QRenderPass);
    return d->m_uniforms;
}

void QRenderPass::setStateSet(Render::DrawStateSet *ss)
{
    Q_D(QRenderPass);
    d->m_stateSet = ss;
}

Render::DrawStateSet *QRenderPass::stateSet() const
{
    Q_D(const QRenderPass);
    return d->m_stateSet;
}

void QRenderPass::addCriterion(RenderPassCriterion *criterion)
{
    Q_D(QRenderPass);
    if (!d->m_criteria.contains(criterion)) {
        d->m_criteria.append(criterion);
        // TO DO: Notify QChangeArbiter
    }
}

void QRenderPass::removeCriterion(RenderPassCriterion *criterion)
{
    Q_D(QRenderPass);
    d->m_criteria.removeOne(criterion);
    // TO DO: Notify QChangeArbiter
}

QList<RenderPassCriterion *> QRenderPass::criteria() const
{
    Q_D(const QRenderPass);
    return d->m_criteria;
}

void QRenderPass::addBinding(QParameterMapper *binding)
{
    Q_D(QRenderPass);
    // TO DO: Notify QChangeArbiter
    if (!d->m_bindings.contains(binding))
        d->m_bindings.append(binding);
}

void QRenderPass::removeBinding(QParameterMapper *binding)
{
    Q_D(QRenderPass);
    // TO DO: Notify QChangeArbiter
    d->m_bindings.removeOne(binding);
}

QList<QParameterMapper *> QRenderPass::bindings() const
{
    Q_D(const QRenderPass);
    return d->m_bindings;
}

} // namespace Qt3D

QT_END_NAMESPACE
