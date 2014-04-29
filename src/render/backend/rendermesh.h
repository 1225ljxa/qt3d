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

#ifndef QT3D_RENDER_RENDERMESH_H
#define QT3D_RENDER_RENDERMESH_H

#include <Qt3DCore/axisalignedboundingbox.h>
#include <Qt3DCore/qchangearbiter.h>
#include <Qt3DCore/qhandle.h>

QT_BEGIN_NAMESPACE

class QReadWriteLock;

namespace Qt3D {

class Mesh;
class RenderPass;
class RendererAspect;
class MeshData;

typedef QHandle<MeshData, 16> HMeshData;

namespace Render {

class RenderNode;
class RenderMaterial;
class RenderTechnique;

class RenderMesh : public QObserverInterface
{
public:
    RenderMesh();

    void setPeer(Mesh *peer);
    void setRendererAspect(RendererAspect *rendererAspect);
    Mesh *peer() const { return m_peer; }

    void sceneChangeEvent(const QSceneChangePtr &e);
    bool meshDirty() const;
    HMeshData meshData() const;
    void setMeshData(HMeshData handle);

    /**
     * @brief mapAttributeNames - resolve mapping of mesh-data attribute
     * names to parameters.
     * @param t
     */
    void mapAttributeNames();

//    virtual void initializeGL(QGraphicsContext* dc);

//    virtual void releaseGL();

protected:
//    virtual void sendDrawingCommands( QGraphicsContext* dc );

//    virtual RenderShader* shader();

//    virtual AxisAlignedBoundingBox boundingBox() const;

private:
    RendererAspect *m_rendererAspect;
    Mesh* m_peer;
    QString m_source;
//    RenderTechnique* m_technique;
//    unsigned int m_pass;

    bool m_meshDirty;
    HMeshData m_meshDataHandle;
    QReadWriteLock *m_lock;
};

} // Render
} // Qt3D

QT_END_NAMESPACE

#endif // QT3D_RENDER_RENDERMESH_H
