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

#include "drawstate.h"

#include <bitset>

#include <QDebug>
#include <QOpenGLContext>

#include "qgraphicscontext.h"

#include "states/blendstate.h"

namespace Qt3D {

namespace Render {

DrawStateSet::DrawStateSet()
{

}

void DrawStateSet::addState(DrawState *ds)
{
    Q_ASSERT(ds);
    m_states.insert(ds);
    m_stateMask |= ds->mask();
}

int DrawStateSet::changeCost(DrawStateSet *previousState)
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
    foreach (DrawState* ds, m_states) {
        // if the other state contains matching, then doesn't
        // contribute to cost at all
        if (previousState->contains(ds)) {
            continue;
        }

        // flat cost for now; could be replaced with a cost() method on
        // DrawState
        cost += 2;
    }

    return cost;
}

void DrawStateSet::apply(QGraphicsContext *gc)
{
    DrawStateSet* previousStates = gc->currentStateSet();

    StateMaskSet invOurState = ~stateMask();
    // generate a mask for each set bit in previous, where we do not have
    // the corresponding bit set.
    StateMaskSet stateToReset = 0;
    if (previousStates)
        stateToReset = previousStates->stateMask() & invOurState;

    resetMasked(stateToReset, gc);

    if (previousStates == m_cachedPrevious) {
        // state-change cache hit
        foreach (DrawState* ds, m_cachedDeltaStates) {
            ds->apply(gc);
        }
    } else {
        // compute deltas and cache for next frame
        m_cachedDeltaStates.clear();
        m_cachedPrevious = previousStates;

        foreach (DrawState* ds, m_states) {
            if (previousStates && previousStates->contains(ds)) {
                continue;
            }

            m_cachedDeltaStates.append(ds);
            ds->apply(gc);
        }
    }
}

StateMaskSet DrawStateSet::stateMask() const
{
    return m_stateMask;
}

void DrawStateSet::resetMasked(StateMaskSet maskOfStatesToReset, QGraphicsContext *gc)
{
    Q_UNUSED(gc);

    if (maskOfStatesToReset & ScissorStateMask) {
        glDisable(GL_SCISSOR_TEST);
    }

    if (maskOfStatesToReset & BlendStateMask) {
        glDisable(GL_BLEND);
    }

    if (maskOfStatesToReset & StencilWriteStateMask) {
        glStencilMask(0);
    }

    if (maskOfStatesToReset & StencilTestStateMask) {
        glDisable(GL_STENCIL_TEST);
    }

    if (maskOfStatesToReset & DepthTestStateMask) {
        glDisable(GL_DEPTH_TEST);
    }

    if (maskOfStatesToReset & DepthWriteStateMask) {
        glDepthMask(GL_TRUE); // reset to default
    }

    if (maskOfStatesToReset & FrontFaceStateMask) {
        glFrontFace(GL_CCW); // reset to default
    }

    if (maskOfStatesToReset & CullFaceStateMask) {
        glDisable(GL_CULL_FACE);
    }
}

bool DrawStateSet::contains(DrawState *ds) const
{
    // trivial reject using the state mask bits
    if (!(ds->mask() & stateMask()))
        return false;

    return m_states.contains(ds);
}

} // Render

} // namespace Qt3D
