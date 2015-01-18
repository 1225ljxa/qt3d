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

#ifndef QT3D_RENDER_RENDERSHADERDATA_P_H
#define QT3D_RENDER_RENDERSHADERDATA_P_H

#include <Qt3DCore/qbackendnode.h>
#include <private/shadervariables_p.h>
#include <Qt3DRenderer/qshaderdata.h>
#include <QMutex>

QT_BEGIN_NAMESPACE

namespace Qt3D {

namespace Render {

class QGraphicsContext;
class UniformBuffer;
class ShaderDataManager;

class RenderShaderData : public QBackendNode
{
public:
    RenderShaderData();
    ~RenderShaderData();

    void updateFromPeer(QNode *peer) Q_DECL_OVERRIDE;
    inline QHash<QString, QVariant> & properties() { return m_properties; }
    inline QHash<QString, QVariant> properties() const { return m_properties; }
    inline QHash<QString, QVariant> updatedProperties() const { return m_updatedProperties; }

    // Called by cleanup job
    inline static QList<QNodeId> updatedShaderDataList() { return m_updatedShaderData; }
    inline static void clearShaderDataList() { return m_updatedShaderData.clear(); }
    void clearUpdate();

    // Call by RenderViewJobs
    void addToClearUpdateList();
    bool needsUpdate();

protected:
    void sceneChangeEvent(const QSceneChangePtr &e) Q_DECL_OVERRIDE;

private:
    QHash<QString, QVariant> m_properties;
    QHash<QString, QVariant> m_updatedProperties;
    PropertyReaderInterfacePtr m_propertyReader;
    QHash<QString, QVariant> m_nestedShaderDataProperties;
    ShaderDataManager *m_manager;
    QMutex *m_mutex;
    static QList<QNodeId> m_updatedShaderData;

    void readPeerProperties(QShaderData *peer);
    void setManager(ShaderDataManager *manager);

    friend class RenderShaderDataFunctor;
};

class RenderShaderDataFunctor : public QBackendNodeFunctor
{
public:
    explicit RenderShaderDataFunctor(ShaderDataManager *manager);

    QBackendNode *create(QNode *frontend) const Q_DECL_FINAL;
    QBackendNode *get(QNode *frontend) const Q_DECL_FINAL;
    void destroy(QNode *frontend) const Q_DECL_FINAL;

private:
    ShaderDataManager *m_manager;
};

} // Render

} // Qt3D

QT_END_NAMESPACE

#endif // QT3D_RENDER_RENDERSHADERDATA_P_H
