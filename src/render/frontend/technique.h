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

#ifndef TECHNIQUE_H
#define TECHNIQUE_H

#include <Qt3DRenderer/qt3drenderer_global.h>
#include <Qt3DCore/node.h>
#include <QOpenGLContext>
#include <Qt3DRenderer/tag.h>
#include <Qt3DRenderer/renderpass.h>
#include <Qt3DCore/qabstracttechnique.h>
#include <Qt3DRenderer/techniquecriterion.h>
#include <QList>
#include <QMap>
#include <QSharedPointer>

// FIXME - move enum somewhere common so don't need to include this here
#include <Qt3DRenderer/quniformvalue.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

class QT3DRENDERERSHARED_EXPORT Parameter : public QObject
{
    Q_OBJECT
    Q_ENUMS(OpenGLTypes)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QString meshAttributeName READ meshAttributeName WRITE setMeshAttributeName NOTIFY meshAttributeNameChanged)
    Q_PROPERTY(int datatype READ datatype() WRITE setDatatype() NOTIFY datatypeChanged())


public:
    // FIXME - sort this by frequency, to minimize the size of the
    // vector in RenderShader. (We want to use compact storage, but we index
    // by this enum, and resize to the largest value)
    enum StandardUniform
    {
        None = -1,

        ModelMatrix = 0,
        ViewMatrix,
        ProjectionMatrix,
        ModelView,
        ModelViewProjection,

        ModelInverse,
        ViewInverse,
        ProjectionInverse,
        ModelViewInverse,
        ModelViewProjectionInverse,

        ModelNormal,
        ModelViewNormal
    };

    enum OpenGLTypes
    {
        Sampler1D = GL_SAMPLER_1D,
        Sampler2D = GL_SAMPLER_2D,
        Sampler3D = GL_SAMPLER_3D,
        SamplerCube = GL_SAMPLER_CUBE,
        Bool = GL_BOOL,
        BoolVec2 = GL_BOOL_VEC2,
        BoolVec3 = GL_BOOL_VEC3,
        BoolVec4 = GL_BOOL_VEC4,
        Double = GL_DOUBLE,
        DoubleVec2 = GL_DOUBLE_VEC2,
        DoubleVec3 = GL_DOUBLE_VEC3,
        DoubleVec4 = GL_DOUBLE_VEC4,
        Float = GL_FLOAT,
        FloatVec2 = GL_FLOAT_VEC2,
        FloatVec3 = GL_FLOAT_VEC3,
        FloatVec4 = GL_FLOAT_VEC4,
        FloatMat4 = GL_FLOAT_MAT4
    };

    Parameter(QObject* parent, QString name, int ty);

    void setName(const QString &name);
    QString name() const
    { return m_name; }

    // permit one extra level of indrection in mesh naming of
    // attributes (glTf at least does this)
    void setMeshAttributeName(QString name);
    QString meshAttributeName() const
    { return m_meshName; }

    bool isStandardUniform() const;

    void setStandardUniform(StandardUniform su);
    StandardUniform standardUniform() const;

    /**
     * @brief setDefaultValue - for non-texture uniform parameters
     * @param dv
     */
    void setDefaultValue(QVariant dv);

    // setUniformValue();

    QVariant value() const;

    int datatype() const
    { return m_type; }
    void setDatatype(int type);

    bool isTextureType() const;

    /**
     * @brief uniformType - map the data type to the primitive uniform type
     * @return
     */
    Render::QUniformValue::Type uniformType() const;
Q_SIGNALS:
    void valueChanged();
    void nameChanged();
    void meshAttributeNameChanged();
    void datatypeChanged();

private:
    QString m_name;
    int m_type;
    QVariant m_value;
    QVariant m_defaultValue;
    QString m_meshName;
    StandardUniform m_standardUniform;
};

class QT3DRENDERERSHARED_EXPORT Technique : public QAbstractTechnique
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    explicit Technique(Node *parent = 0);

    void addCriterion(TechniqueCriterion *criterion);
    void removeCriterion(TechniqueCriterion *criterion);
    QVariant criterionValue(const QString &customTypeName) const;
    QVariant criterionValue(TechniqueCriterion::CriterionType type);
    QList<TechniqueCriterion *> criteria() const;
    void clearCriteria();
    bool containsCriterion(const QString &customTypeName) const;
    bool containsCriterion(TechniqueCriterion::CriterionType type) const;

    void addPass(RenderPass* pass);
    void removePass(RenderPass *pass);
    QList<RenderPass *> renderPasses() const;

    // QQmlListProperty<Qt3D::Parameter> parameters();

    void addParameter(Parameter *p);
    void removeParameter(Parameter *p);
    QList<Parameter *> parameters() const
    { return m_parameters; }

    Parameter* parameterByName(QString name) const;

Q_SIGNALS:
    void nameChanged();

private:

    QList<TechniqueCriterion *> m_criteriaList;
    QList<Parameter *> m_parameters;
    QList<RenderPass *> m_renderPasses;
};

}

QT_END_NAMESPACE

#endif // TECHNIQUE_H
