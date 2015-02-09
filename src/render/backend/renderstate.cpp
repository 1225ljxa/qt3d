/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Copyright (C) 2015 The Qt Company Ltd and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "renderstate_p.h"

#include <bitset>

#include <QDebug>
#include <QOpenGLContext>

#include <Qt3DRenderer/private/qgraphicscontext_p.h>
#include <Qt3DRenderer/private/blendstate_p.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {
namespace Render {

RenderStateSet::RenderStateSet()
    : m_stateMask(0)
    , m_cachedPrevious(0)
{

}

void RenderStateSet::addState(RenderState *ds)
{
    Q_ASSERT(ds);
    m_states.insert(ds);
    m_stateMask |= ds->mask();
}

int RenderStateSet::changeCost(RenderStateSet *previousState)
{
    if (previousState == this)
        return 0;

    int cost = 0;

// first, find cost of any resets
    StateMaskSet invOurState = ~stateMask();
    StateMaskSet stateToReset = previousState->stateMask() & invOurState;

    std::bitset<64> bs(stateToReset);
    cost += bs.count();

// now, find out how many states we're changing
    Q_FOREACH (RenderState* ds, m_states) {
        // if the other state contains matching, then doesn't
        // contribute to cost at all
        if (previousState->contains(ds)) {
            continue;
        }

        // flat cost for now; could be replaced with a cost() method on
        // RenderState
        cost += 2;
    }

    return cost;
}

void RenderStateSet::apply(QGraphicsContext *gc)
{
    RenderStateSet* previousStates = gc->currentStateSet();

    StateMaskSet invOurState = ~stateMask();
    // generate a mask for each set bit in previous, where we do not have
    // the corresponding bit set.
    StateMaskSet stateToReset = 0;
    if (previousStates)
        stateToReset = previousStates->stateMask() & invOurState;

    resetMasked(stateToReset, gc);

    if (m_cachedPrevious && previousStates == m_cachedPrevious) {
        // state-change cache hit
        foreach (RenderState* ds, m_cachedDeltaStates) {
            ds->apply(gc);
        }
    } else {
        // compute deltas and cache for next frame
        m_cachedDeltaStates.clear();
        m_cachedPrevious = previousStates;

        foreach (RenderState* ds, m_states) {
            if (previousStates && previousStates->contains(ds)) {
                continue;
            }

            m_cachedDeltaStates.append(ds);
            ds->apply(gc);
        }
    }
}

StateMaskSet RenderStateSet::stateMask() const
{
    return m_stateMask;
}

void RenderStateSet::resetMasked(StateMaskSet maskOfStatesToReset, QGraphicsContext *gc)
{
    // TO DO -> Call gcHelper methods instead of raw GL
    // QOpenGLFunctions shouldn't be used here directly
    QOpenGLFunctions *funcs = gc->openGLContext()->functions();

    if (maskOfStatesToReset & ScissorStateMask) {
        funcs->glDisable(GL_SCISSOR_TEST);
    }

    if (maskOfStatesToReset & BlendStateMask) {
        funcs->glDisable(GL_BLEND);
    }

    if (maskOfStatesToReset & StencilWriteStateMask) {
        funcs->glStencilMask(0);
    }

    if (maskOfStatesToReset & StencilTestStateMask) {
        funcs->glDisable(GL_STENCIL_TEST);
    }

    if (maskOfStatesToReset & DepthTestStateMask) {
        funcs->glDisable(GL_DEPTH_TEST);
    }

    if (maskOfStatesToReset & DepthWriteStateMask) {
        funcs->glDepthMask(GL_TRUE); // reset to default
    }

    if (maskOfStatesToReset & FrontFaceStateMask) {
        funcs->glFrontFace(GL_CCW); // reset to default
    }

    if (maskOfStatesToReset & CullFaceStateMask) {
        funcs->glDisable(GL_CULL_FACE);
    }

    if (maskOfStatesToReset & DitheringStateMask) {
        funcs->glDisable(GL_DITHER);
    }

    if (maskOfStatesToReset & AlphaCoverageStateMask) {
        gc->disableAlphaCoverage();
    }

    if (maskOfStatesToReset & PolygonOffsetStateMask) {
        funcs->glDisable(GL_POLYGON_OFFSET_FILL);
    }

    if (maskOfStatesToReset & ColorStateMask) {
        funcs->glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    }
}

bool RenderStateSet::contains(RenderState *ds) const
{
    // trivial reject using the state mask bits
    if (!(ds->mask() & stateMask()))
        return false;

    return m_states.contains(ds);
}

} // Render
} // namespace Qt3D

QT_END_NAMESPACE
