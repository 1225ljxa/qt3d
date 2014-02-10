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

#ifndef RENDERMESH_H
#define RENDERMESH_H

#include "drawable.h"

#include <meshdata.h>
#include <axisalignedboundingbox.h>

#include <QOpenGLVertexArrayObject>
#include <QMatrix4x4>

class QOpenGLFunctions_3_3_Core;

namespace Qt3D {

class Mesh;
class RenderNode;
class RenderPass;
class RenderMaterial;
class RenderTechnique;

class RenderMesh : public Drawable
{
public:
    explicit RenderMesh(Mesh* peer);

    void setPeer(Mesh* peer);

    void setData(MeshDataPtr mesh);

    void setTechniqueAndPass(RenderTechnique* tech, unsigned int pass);

    void setMaterial(RenderMaterial* rmat);

    void setInstanceCount(unsigned int count);

    void setModelMatrix(const QMatrix4x4& mm);

    virtual DrawStateSet* stateSet();

    /**
     * @brief mapAttributeNames - resolve mapping of mesh-data attribute
     * names to parameters.
     * @param t
     */
    void mapAttributeNames();

    virtual void initializeGL(QGraphicsContext* dc);

    virtual void releaseGL();

protected:
    virtual void sendDrawingCommands( QGraphicsContext* dc );

    virtual RenderShader* shader();

    virtual AxisAlignedBoundingBox boundingBox() const;

private:


    Mesh* m_peer;

    MeshDataPtr m_meshData;

    RenderTechnique* m_technique;
    unsigned int m_pass;

    RenderMaterial* m_material;

    bool m_drawIndexed;
    bool m_meshDirty;
    unsigned int m_instanceCount;

    QOpenGLVertexArrayObject m_vao;

    QOpenGLFunctions_3_3_Core *m_funcs;

    QMatrix4x4 m_modelMatrix;
};

}

#endif // RENDERMESH_H
