/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
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

#ifndef QT3DRENDER_RENDER_SHADERDATA_P_H
#define QT3DRENDER_RENDER_SHADERDATA_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <Qt3DCore/qbackendnode.h>
#include <private/shadervariables_p.h>
#include <Qt3DRenderer/qshaderdata.h>
#include <QMutex>
#include <QMatrix4x4>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

namespace Render {

class GraphicsContext;
class UniformBuffer;
class ShaderDataManager;

class Q_AUTOTEST_EXPORT ShaderData : public Qt3DCore::QBackendNode
{
public:
    ShaderData();
    ~ShaderData();

    void updateFromPeer(Qt3DCore::QNode *peer) Q_DECL_OVERRIDE;
    inline QHash<QString, QVariant> properties() const { return m_properties; }
    inline QHash<QString, QVariant> updatedProperties() const { return m_updatedProperties; }

    // Called by cleanup job
    inline static QList<Qt3DCore::QNodeId> updatedShaderDataList() { return m_updatedShaderData; }
    inline static void clearShaderDataList() { return m_updatedShaderData.clear(); }
    void clearUpdate();

    // Call by RenderViewJobs
    void addToClearUpdateList();
    bool needsUpdate(const QMatrix4x4 &viewMatrix);

    void updateTransformedProperties(const QMatrix4x4 &nodeWordlTransform);

protected:
    void sceneChangeEvent(const Qt3DCore::QSceneChangePtr &e) Q_DECL_OVERRIDE;

private:
    // 1 to 1 match with frontend properties, modified only by sceneChangeEvent
    QHash<QString, QVariant> m_originalProperties;
    // 1 to 1 match with frontend properties apart from Transformed
    // properties which contain the matrices product
    QHash<QString, QVariant> m_properties;
    // only updated properties, Transformed properties have the same
    // value as in m_properties
    QHash<QString, QVariant> m_updatedProperties;
    PropertyReaderInterfacePtr m_propertyReader;
    QHash<QString, QVariant> m_nestedShaderDataProperties;
    QHash<QString, QShaderData::TransformType> m_transformedProperties;
    ShaderDataManager *m_manager;
    QMutex *m_mutex;
    static QList<Qt3DCore::QNodeId> m_updatedShaderData;
    QMatrix4x4 m_worldMatrix;
    QMatrix4x4 m_viewMatrix;

    void readPeerProperties(QShaderData *peer);
    void setManager(ShaderDataManager *manager);

    friend class RenderShaderDataFunctor;
};

class RenderShaderDataFunctor : public Qt3DCore::QBackendNodeFunctor
{
public:
    explicit RenderShaderDataFunctor(ShaderDataManager *manager);

    Qt3DCore::QBackendNode *create(Qt3DCore::QNode *frontend, const Qt3DCore::QBackendNodeFactory *factory) const Q_DECL_FINAL;
    Qt3DCore::QBackendNode *get(const Qt3DCore::QNodeId &id) const Q_DECL_FINAL;
    void destroy(const Qt3DCore::QNodeId &id) const Q_DECL_FINAL;

private:
    ShaderDataManager *m_manager;
};

} // namespace Render

} // namespace Qt3DRender

QT_END_NAMESPACE

Q_DECLARE_METATYPE(Qt3DRender::Render::ShaderData*)

#endif // QT3DRENDER_RENDER_SHADERDATA_P_H
