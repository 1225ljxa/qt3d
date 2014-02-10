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

#ifndef RENDERMATERIAL_H
#define RENDERMATERIAL_H

#include <QVariant>

#include <qchangearbiter.h>

#include <quniformvalue.h>
#include <rendertextureprovider.h>

namespace Qt3D
{

class Material;
class RenderTechnique;
class QGraphicsContext;
class RendererAspect;

class RenderMaterial : public QObserverInterface
{
public:
    RenderMaterial(RendererAspect *rendererAspect);
    ~RenderMaterial();

    void setPeer(Material* mat);

    void setEffectName(QString nm);

    void setParameter(QString paramName, QVariant value);

    void setTextureParameter(QString paramName, RenderTexturePtr tex);

    void syncParametersFromPeer();

    /**
     * @brief setTechnique - specify the effect technique to use for
     * rendering this material.
     * @param rt
     */
    void setTechnique(RenderTechnique* rt);
    RenderTechnique* technique() const;

    void setUniformsForPass(unsigned int pass, QGraphicsContext* gc);

    void setTextureProvider(RenderTextureProvider *rtp);

    void sceneChangeEvent(const QSceneChangePtr &e) Q_DECL_OVERRIDE;

private:
    RendererAspect *m_rendererAspect;
    Material* m_peer;
    QString m_effectName;
    RenderTechnique* m_technique;
    RenderTextureProvider* m_textureProvider;

    QVector<Render::QUniformPack*> m_packs;
};

}

#endif // RENDERMATERIAL_H
