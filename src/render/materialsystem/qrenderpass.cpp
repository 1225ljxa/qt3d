/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Copyright (C) 2016 The Qt Company Ltd and/or its subsidiary(-ies).
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

#include "qrenderpass.h"
#include "qrenderpass_p.h"
#include "qparameter.h"
#include "qannotation.h"
#include "qscenepropertychange.h"
#include "qrenderstate.h"
#include "private/qnode_p.h"

QT_BEGIN_NAMESPACE

using namespace Qt3DCore;

namespace Qt3DRender {

QRenderPassPrivate::QRenderPassPrivate()
    : QNodePrivate()
    , m_shader(Q_NULLPTR)
{
}

void QRenderPass::copy(const QNode *ref)
{
    QNode::copy(ref);
    const QRenderPass *other = static_cast<const QRenderPass*>(ref);
    d_func()->m_shader = qobject_cast<QShaderProgram *>(QNode::clone(other->d_func()->m_shader));

    Q_FOREACH (QAnnotation *crit, other->d_func()->m_annotationList)
        addAnnotation(qobject_cast<QAnnotation *>(QNode::clone(crit)));
    Q_FOREACH (QRenderState *renderState, other->d_func()->m_renderStates)
        addRenderState(qobject_cast<QRenderState *>(QNode::clone(renderState)));
    Q_FOREACH (QParameter *p, other->d_func()->m_parameters)
        addParameter(qobject_cast<QParameter *>(QNode::clone(p)));
}

QRenderPass::QRenderPass(QNode *parent)
    : QNode(*new QRenderPassPrivate, parent)
{
}

QRenderPass::~QRenderPass()
{
    QNode::cleanup();
}

/*! \internal */
QRenderPass::QRenderPass(QRenderPassPrivate &dd, QNode *parent)
    : QNode(dd, parent)
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

/*!
 * Sets the pass's \a shaderProgram. This posts a ComponentUpdated
 * QScenePropertyChange to the QChangeArbiter. The value is set to
 * the \a shaderProgram and the property name to \c {"shaderProgram"}.
 */
void QRenderPass::setShaderProgram(QShaderProgram *shaderProgram)
{
    Q_D(QRenderPass);
    if (d->m_shader != shaderProgram) {

        if (d->m_shader != Q_NULLPTR && d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr e(new QScenePropertyChange(NodeRemoved, QSceneChange::Node, id()));
            e->setPropertyName("shaderProgram");
            e->setValue(QVariant::fromValue(d->m_shader->id()));
            d->notifyObservers(e);
        }

        d->m_shader = shaderProgram;
        emit shaderProgramChanged(shaderProgram);

        // We need to add it as a child of the current node if it has been declared inline
        // Or not previously added as a child of the current node so that
        // 1) The backend gets notified about it's creation
        // 2) When the current node is destroyed, it gets destroyed as well
        if (!shaderProgram->parent())
            shaderProgram->setParent(this);

        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr e(new QScenePropertyChange(NodeAdded, QSceneChange::Node, id()));
            e->setPropertyName("shaderProgram");
            e->setValue(QVariant::fromValue(shaderProgram->id()));
            d->notifyObservers(e);
        }
    }
}

QShaderProgram *QRenderPass::shaderProgram() const
{
    Q_D(const QRenderPass);
    return d->m_shader;
}

void QRenderPass::addAnnotation(QAnnotation *annotation)
{
    Q_D(QRenderPass);
    if (!d->m_annotationList.contains(annotation)) {
        d->m_annotationList.append(annotation);

        // We need to add it as a child of the current node if it has been declared inline
        // Or not previously added as a child of the current node so that
        // 1) The backend gets notified about it's creation
        // 2) When the current node is destroyed, it gets destroyed as well
        if (!annotation->parent())
            annotation->setParent(this);

        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr change(new QScenePropertyChange(NodeAdded, QSceneChange::Node, id()));
            change->setPropertyName("annotation");
            change->setValue(QVariant::fromValue(annotation->id()));
            d->notifyObservers(change);
        }
    }
}

void QRenderPass::removeAnnotation(QAnnotation *annotation)
{
    Q_D(QRenderPass);
    if (d->m_changeArbiter != Q_NULLPTR) {
        QScenePropertyChangePtr change(new QScenePropertyChange(NodeRemoved, QSceneChange::Node, id()));
        change->setPropertyName("annotation");
        change->setValue(QVariant::fromValue(annotation->id()));
        d->notifyObservers(change);
    }
    d->m_annotationList.removeOne(annotation);
}

QList<QAnnotation *> QRenderPass::annotations() const
{
    Q_D(const QRenderPass);
    return d->m_annotationList;
}

/*!
 * Adds a Qt3DCore::QRenderState \a state to the rendering pass. That implies that
 * when the pass is executed at render time, the globally set render state will
 * be modifed by the states defined locally by the Qt3DRender::QRenderPass.
 *
 * \note not defining any Qt3DCore::QRenderState in a pass will result in the pass using
 * the globally set render state for a given FrameGraph branch execution path.
 */
void QRenderPass::addRenderState(QRenderState *state)
{
    Q_D(QRenderPass);

    if (!d->m_renderStates.contains(state)) {
        d->m_renderStates.append(state);

        if (!state->parent())
            state->setParent(this);

        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr change(new QScenePropertyChange(NodeAdded, QSceneChange::Node, id()));
            change->setPropertyName("renderState");
            change->setValue(QVariant::fromValue(QNodePtr(QNode::clone(state))));
            d->notifyObservers(change);
        }
    }
}

/*!
 * Removes \a state from the Qt3DRender::QRenderPass local render state.
 */
void QRenderPass::removeRenderState(QRenderState *state)
{
    Q_D(QRenderPass);
    if (d->m_changeArbiter != Q_NULLPTR) {
        QScenePropertyChangePtr change(new QScenePropertyChange(NodeRemoved, QSceneChange::Node, id()));
        change->setPropertyName("renderState");
        change->setValue(QVariant::fromValue(state->id()));
        d->notifyObservers(change);
    }
    d->m_renderStates.removeOne(state);
}

/*!
 * Returns the list of Qt3DCore::QRenderState state objects making up the render
 * state of the Qt3DRender::QRenderPass.
 */
QList<QRenderState *> QRenderPass::renderStates() const
{
    Q_D(const QRenderPass);
    return d->m_renderStates;
}

void QRenderPass::addParameter(QParameter *parameter)
{
    Q_D(QRenderPass);
    if (!d->m_parameters.contains(parameter)) {
        d->m_parameters.append(parameter);

        // We need to add it as a child of the current node if it has been declared inline
        // Or not previously added as a child of the current node so that
        // 1) The backend gets notified about it's creation
        // 2) When the current node is destroyed, the child parameters get destroyed as well
        if (!parameter->parent())
            parameter->setParent(this);

        if (d->m_changeArbiter != Q_NULLPTR) {
            QScenePropertyChangePtr change(new QScenePropertyChange(NodeAdded, QSceneChange::Node, id()));
            change->setPropertyName("parameter");
            change->setValue(QVariant::fromValue(parameter->id()));
            d->notifyObservers(change);
        }
    }
}

void QRenderPass::removeParameter(QParameter *parameter)
{
    Q_D(QRenderPass);

    if (d->m_changeArbiter != Q_NULLPTR) {
        QScenePropertyChangePtr change(new QScenePropertyChange(NodeRemoved, QSceneChange::Node, id()));
        change->setPropertyName("parameter");
        change->setValue(QVariant::fromValue(parameter->id()));
        d->notifyObservers(change);
    }
    d->m_parameters.removeOne(parameter);
}


QList<QParameter *> QRenderPass::parameters() const
{
    Q_D(const QRenderPass);
    return d->m_parameters;
}

} // namespace Qt3DRender

QT_END_NAMESPACE
