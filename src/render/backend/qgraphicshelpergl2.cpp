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

#ifndef QT_OPENGL_ES_2

#include "qgraphicshelpergl2.h"
#include <QOpenGLFunctions_2_0>

QT_BEGIN_NAMESPACE

namespace Qt3D {
namespace Render {

QGraphicsHelperGL2::QGraphicsHelperGL2() :
    m_funcs(0)
{

}

void QGraphicsHelperGL2::initializeHelper(QOpenGLContext *context,
                                         QAbstractOpenGLFunctions *functions)
{
    Q_UNUSED(context)
    m_funcs = static_cast<QOpenGLFunctions_2_0*>(functions);
    Q_ASSERT(m_funcs->initializeOpenGLFunctions());
    // Check Vertex Array Object extension is present
    Q_ASSERT(context->hasExtension(QByteArrayLiteral("GL_ARB_vertex_array_object")));
}

void QGraphicsHelperGL2::drawElementsInstanced(GLenum primitiveType,
                                              GLsizei primitiveCount,
                                              GLint indexType,
                                              void *indices,
                                              GLsizei instances)
{
    for (GLint i = 0; i < instances; i++)
        drawElements(primitiveType,
                           primitiveCount,
                           indexType,
                           indices);
}

void QGraphicsHelperGL2::drawArraysInstanced(GLenum primitiveType,
                                            GLint first,
                                            GLsizei count,
                                            GLsizei instances)
{
    for (GLint i = 0; i < instances; i++)
        drawArrays(primitiveType,
                         first,
                         count);
}

void QGraphicsHelperGL2::drawElements(GLenum primitiveType,
                                     GLsizei primitiveCount,
                                     GLint indexType,
                                     void *indices)
{
    m_funcs->glDrawElements(primitiveType,
                                  primitiveCount,
                                  indexType,
                                  indices);
}

void QGraphicsHelperGL2::drawArrays(GLenum primitiveType,
                                   GLint first,
                                   GLsizei count)
{
    m_funcs->glDrawArrays(primitiveType,
                                first,
                                count);
}

void QGraphicsHelperGL2::useProgram(GLuint programId)
{
    m_funcs->glUseProgram(programId);
}

void QGraphicsHelperGL2::vertexAttribDivisor(GLuint index,
                                            GLuint divisor)
{
    Q_UNUSED(index)
    Q_UNUSED(divisor)
}

void QGraphicsHelperGL2::blendEquation(GLenum mode)
{
    m_funcs->glBlendEquation(mode);
}

} // Render
} // Qt3D

QT_END_NAMESPACE

#endif // !QT_OPENGL_ES_2
