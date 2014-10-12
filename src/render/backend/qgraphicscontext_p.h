/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Copyright (C) 2014 Digia Plc and/or its subsidiary(-ies).
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

#ifndef QT3D_RENDER_QGRAPHICSCONTEXT_H
#define QT3D_RENDER_QGRAPHICSCONTEXT_H

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QHash>
#include <QColor>
#include <QMatrix4x4>
#include <QBitArray>
#include <Qt3DRenderer/private/quniformvalue_p.h>
#include <Qt3DRenderer/private/qmeshdata_p.h>
#include <Qt3DRenderer/qattribute.h>
#include <Qt3DRenderer/qbuffer.h>
#include <Qt3DRenderer/qclearbuffer.h>

QT_BEGIN_NAMESPACE

class QOpenGLShaderProgram;
class QAbstractOpenGLFunctions;

namespace Qt3D {

class QOpenGLFilter;

namespace Render {

class Renderer;
class QGraphicsHelperInterface;
class RenderStateSet;
class RenderShader;
class RenderMaterial;
class RenderTexture;
class RenderCommand;
class RenderTarget;
class AttachmentPack;

enum TextureScope
{
    TextureScopeMaterial = 0,
    TextureScopeTechnique
    // per-pass for deferred rendering?
};

typedef QPair<QString, int> NamedUniformLocation;

class QGraphicsContext
{
public:
    QGraphicsContext();
    ~QGraphicsContext();

    void setSurface(QSurface* s);

    int id() const; // unique, small integer ID of this context

    bool beginDrawing(const QColor &clearColor);
    void clearBackBuffer(QClearBuffer::BufferType buffers);
    void endDrawing();

    void setViewport(const QRectF &viewport);
    QRectF viewport() const { return m_viewport; }

    /**
     * @brief releaseGL - release all OpenGL objects associated with
     * this context
     */
    void releaseOpenGL();
    void setOpenGLContext(QOpenGLContext* ctx);
    QOpenGLContext *openGLContext() { return m_gl; }

    void activateShader(RenderShader* shader);

    void activateRenderTarget(RenderTarget *renderTarget, const AttachmentPack &attachments);

    RenderMaterial* activeMaterial() const
    { return m_material; }

    void setActiveMaterial(RenderMaterial* rmat);

    void executeCommand(const RenderCommand *command);

    /**
     * @brief activeShader
     * @return
     */
    QOpenGLShaderProgram* activeShader();

    void setRenderer(Renderer *renderer);

    void specifyAttribute(QString nm, AttributePtr attr);

    void specifyIndices(AttributePtr attr);

    void setUniforms(QUniformPack &uniforms);

    /**
     * @brief glBufferFor - given a client-side (CPU) buffer, provide the
     * context-specific object. Initial call will create the buffer.
     * @param buf
     * @return
     */
    QOpenGLBuffer glBufferFor(BufferPtr buf);

    /**
     * @brief activateTexture - make a texture active on a hardware unit
     * @param tex - the texture to activate
     * @param onUnit - option, specify the explicit unit to activate on
     * @return - the unit the texture was activated on
     */
    int activateTexture(TextureScope scope, RenderTexture* tex, int onUnit = -1);

    void deactivateTexture(RenderTexture *tex);

    void setCurrentStateSet(RenderStateSet* ss);
    RenderStateSet *currentStateSet() const;
    QOpenGLFilter *contextInfo() const;

    // Wrapper methods
    void    drawElementsInstanced(GLenum primitiveType, GLsizei primitiveCount, GLint indexType, void * indices, GLsizei instances);
    void    drawArraysInstanced(GLenum primitiveType, GLint first, GLsizei count, GLsizei instances);
    void    drawElements(GLenum primitiveType, GLsizei primitiveCount, GLint indexType, void * indices);
    void    drawArrays(GLenum primitiveType, GLint first, GLsizei count);
    void    setVerticesPerPatch(GLint verticesPerPatch);
    void    blendEquation(GLenum mode);
    void    alphaTest(GLenum mode1, GLenum mode2);
    void    depthTest(GLenum mode);
    void    depthMask(GLenum mode);
    void    cullFace(GLenum mode);
    void    frontFace(GLenum mode);
    void    bindFragOutputs(GLuint shader, const QHash<QString, int> &outputs);
    void    bindUniform(const QVariant &v, const ShaderUniform &description);
    void    enableAlphaCoverage();
    void    disableAlphaCoverage();

    // Helper methods
    static GLint elementType(GLint type);
    static GLint tupleSizeFromType(GLint type);
    static GLuint byteSizeFromType(GLint type);

private:
    void initialize();

    void decayTextureScores();

    GLint assignUnitForTexture(RenderTexture* tex);
    void deactivateTexturesWithScope(TextureScope ts);

    void resolveHighestOpenGLFunctions();

    void bindFrameBufferAttachmentHelper(GLuint fboId, const AttachmentPack &attachments);

    bool m_initialized;
    const unsigned int m_id;
    QOpenGLContext *m_gl;
    QSurface *m_surface;
    QGraphicsHelperInterface *m_glHelper;

    RenderShader *m_activeShader;
    QHash<RenderShader *, QOpenGLShaderProgram *> m_shaderHash;
    QHash<BufferPtr, QOpenGLBuffer> m_bufferHash;
    QHash<QUuid, GLuint> m_renderTargets;
    QHash<GLuint, QSize> m_renderTargetsSize;

    // active textures, indexed by texture unit
    QVector<RenderTexture *> m_activeTextures;
    QBitArray m_pinnedTextureUnits;
    QVector<TextureScope> m_textureScopes;

    // recency score for all render-textures we've seen. Higher scores
    // mean more recently used.
    QHash<RenderTexture *, int> m_textureScores;

    RenderMaterial* m_material;
    QRectF m_viewport;
    GLuint m_activeFBO;

    RenderStateSet* m_stateSet;

    Renderer *m_renderer;
    QOpenGLFilter *m_contextInfo;
};

} // Render
} // Qt3D

QT_END_NAMESPACE

#endif // QT3D_RENDER_QGRAPHICSCONTEXT_H
