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

#include "qrendertargetselector.h"
#include "qrendertargetselector_p.h"
#include <Qt3DRender/qrendertarget.h>
#include <Qt3DCore/qscenepropertychange.h>
#include <Qt3DRender/private/qrenderpass_p.h>

QT_BEGIN_NAMESPACE

using namespace Qt3DCore;

namespace Qt3DRender {

QRenderTargetSelectorPrivate::QRenderTargetSelectorPrivate()
    : QFrameGraphNodePrivate()
    , m_target(Q_NULLPTR)
{
}

void QRenderTargetSelector::copy(const QNode *ref)
{
    QFrameGraphNode::copy(ref);

    const QRenderTargetSelector *other = static_cast<const QRenderTargetSelector*>(ref);

    setOutputs(other->outputs());
    if (other->d_func()->m_target)
        setTarget(qobject_cast<QRenderTarget *>(QNode::clone(other->d_func()->m_target)));
}

QRenderTargetSelector::QRenderTargetSelector(QNode *parent)
    : QFrameGraphNode(*new QRenderTargetSelectorPrivate, parent)
{
}

QRenderTargetSelector::~QRenderTargetSelector()
{
    QNode::cleanup();
}

void QRenderTargetSelector::setTarget(QRenderTarget *target)
{
    Q_D(QRenderTargetSelector);
    if (d->m_target != target) {
        d->m_target = target;

        // For inline declaration cases
        if (target != Q_NULLPTR && !target->parent())
            target->setParent(this);
        emit targetChanged(target);
    }
}

QRenderTarget *QRenderTargetSelector::target() const
{
    Q_D(const QRenderTargetSelector);
    return d->m_target;
}

/*!
 * \internal
 * Sets the draw buffers \a buffers to be used. The draw buffers should be
 * matching the Qt3DRender::QRenderTargetOutput::RenderAttachmentType
 * defined in the attachments of the Qt3DRender::QRenderTarget associated to the
 * Qt3DRender::QRenderTargetSelector instance.
 *
 * \note At render time, if no draw buffer has been specified, the renderer will
 * default to using all the attachments' draw buffers.
 *
 */
void QRenderTargetSelector::setOutputs(const QList<QRenderTargetOutput::AttachmentPoint> &buffers)
{
    Q_D(QRenderTargetSelector);
    if (buffers != d->m_outputs) {
        d->m_outputs = buffers;

        if (d->m_changeArbiter) {
            QScenePropertyChangePtr change(new QScenePropertyChange(NodeUpdated, QSceneChange::Node, id()));
            change->setPropertyName("outputs");
            change->setValue(QVariant::fromValue(d->m_outputs));
            d->notifyObservers(change);
        }
    }
}

/*!
 * Returns the list of draw buffers for the current Qt3DRender::QRenderTargetSelector instance.
 */
QList<QRenderTargetOutput::AttachmentPoint> QRenderTargetSelector::outputs() const
{
    Q_D(const QRenderTargetSelector);
    return d->m_outputs;
}

QRenderTargetSelector::QRenderTargetSelector(QRenderTargetSelectorPrivate &dd, QNode *parent)
    : QFrameGraphNode(dd, parent)
{
}

Qt3DCore::QNodeCreatedChangeBasePtr QRenderTargetSelector::createNodeCreationChange() const
{
    auto creationChange = Qt3DCore::QNodeCreatedChangePtr<QRenderTargetSelectorData>::create(this);
    auto &data = creationChange->data;
    Q_D(const QRenderTargetSelector);
    data.targetId = qIdForNode(d->m_target);
    data.outputs = d->m_outputs.toVector();
    return creationChange;
}

} // namespace Qt3DRender

QT_END_NAMESPACE
