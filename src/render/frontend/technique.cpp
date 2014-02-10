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

#include "technique.h"

#include <QDebug>
#include <QOpenGLContext>

namespace Qt3D {

Technique::Technique(Node *parent)
    : Node(parent)
{
}

QQmlListProperty<Qt3D::Tag> Technique::tags()
{
    return QQmlListProperty<Qt3D::Tag>(this, 0,
                                       &Technique::appendTag,
                                       &Technique::tagCount,
                                       &Technique::tagAt,
                                       &Technique::clearTags );
}

void Technique::addTag(const QString &name, const QVariant &value)
{
    Tag *tag = new Tag(this);
    tag->setName(name);
    tag->setValue(value);
    m_tags.insert(name, tag);
    m_tagList.append(tag);
    emit tagsChanged();
}

void Technique::removeTag(const QString &name)
{
    Tag *tag = m_tags.value(name);
    m_tags.remove(name);
    m_tagList.removeOne(tag);
    if (tag) {
        delete tag;
        emit tagsChanged();
    }
}

void Technique::setTagValue(const QString &name, const QVariant &value)
{
    Tag *tag = m_tags.value(name);
    if (tag)
        tag->setValue(value);
}

QVariant Technique::tagValue(const QString &name) const
{
    return m_tags.value(name)->value();
}

bool Technique::containsTag(const QString &name) const
{
    return m_tags.contains(name);
}

void Technique::addPass(RenderPass *pass)
{
    Q_ASSERT(pass);
    pass->setParent(this);
    m_renderPasses.append(pass);
    emit renderPassesChanged();
}

QList<RenderPass *> Technique::renderPasses() const
{
    return m_renderPasses;
}

void Technique::appendTag(QQmlListProperty<Tag> *list, Tag *tag)
{
    Technique *technique = qobject_cast<Technique *>(list->object);
    if (technique) {
        tag->setParent(technique);
        technique->m_tags.insert(tag->name(), tag);
        technique->m_tagList.append(tag);
        emit technique->tagsChanged();
    }
}

Tag *Technique::tagAt(QQmlListProperty<Tag> *list, int index)
{
    Technique *technique = qobject_cast<Technique *>(list->object);
    if (technique)
        return technique->m_tagList.value(index);
    return 0;
}

int Technique::tagCount(QQmlListProperty<Tag> *list)
{
    Technique *technique = qobject_cast<Technique *>(list->object);
    if (technique)
        return technique->m_tagList.size();
    return 0;
}

void Technique::clearTags(QQmlListProperty<Tag> *list)
{
    Technique *technique = qobject_cast<Technique *>(list->object);
    if (technique) {
        technique->m_tags.clear();
        technique->m_tagList.clear();
        emit technique->tagsChanged();
    }
}

QQmlListProperty<Qt3D::RenderPass> Technique::renderPassList()
{
    return QQmlListProperty<Qt3D::RenderPass>(this, 0,
                                              &Technique::appendRenderPass,
                                              &Technique::renderPassCount,
                                              &Technique::renderPassAt,
                                              &Technique::clearRenderPasses);
}

void Technique::addParameter(Parameter *p)
{
    Q_CHECK_PTR(p);
    m_parameters.append(p);
}

Parameter *Technique::parameterByName(QString name) const
{
    foreach (Parameter* p, m_parameters) {
        if (p->name() == name)
            return p;
    }

    return NULL;
}

void Technique::appendRenderPass(QQmlListProperty<RenderPass> *list, RenderPass *renderPass)
{
    Technique *technique = qobject_cast<Technique *>(list->object);
    if (technique) {
        technique->addPass(renderPass);
    }
}

RenderPass *Technique::renderPassAt(QQmlListProperty<RenderPass> *list, int index)
{
    Technique *technique = qobject_cast<Technique *>(list->object);
    if (technique)
        return technique->m_renderPasses.value(index);
    return 0;
}

int Technique::renderPassCount(QQmlListProperty<RenderPass> *list)
{
    Technique *technique = qobject_cast<Technique *>(list->object);
    if (technique)
        return technique->m_renderPasses.size();
    return 0;
}

void Technique::clearRenderPasses(QQmlListProperty<RenderPass> *list)
{
    Technique *technique = qobject_cast<Technique *>(list->object);
    if (technique) {
        technique->m_renderPasses.clear();
        emit technique->renderPassesChanged();
    }
}

Parameter::Parameter(QObject *parent, QString name, int ty) :
    QObject(parent),
    m_name(name),
    m_type(ty),
    m_standardUniform(None)
{

}

void Parameter::setMeshAttributeName(QString name)
{
    m_meshName = name;
}

bool Parameter::isStandardUniform() const
{
    return (m_standardUniform != None);
}

void Parameter::setStandardUniform(Parameter::StandardUniform su)
{
    m_standardUniform = su;
}

Parameter::StandardUniform Parameter::standardUniform() const
{
    return m_standardUniform;
}

bool Parameter::isTextureType() const
{
    switch (m_type) {
    case GL_SAMPLER_1D:
    case GL_SAMPLER_2D:
    case GL_SAMPLER_3D:
    case GL_SAMPLER_CUBE:
        return true;
    default:
        return false;
    }
}

Render::QUniformValue::Type Parameter::uniformType() const
{
    switch (m_type) {
    case GL_BOOL:
    case GL_BOOL_VEC2:
    case GL_BOOL_VEC3:
    case GL_BOOL_VEC4:
        return Render::QUniformValue::Bool;

    // integers!

    case GL_FLOAT:
    case GL_FLOAT_VEC2:
    case GL_FLOAT_VEC3:
    case GL_FLOAT_VEC4:
    case GL_FLOAT_MAT4:
        return Render::QUniformValue::Float;

    case GL_DOUBLE:
    case GL_DOUBLE_VEC2:
    case GL_DOUBLE_VEC3:
    case GL_DOUBLE_VEC4:
        return Render::QUniformValue::Double;

    default:
        qWarning() << Q_FUNC_INFO << "couldn't map datatype:" << QString::number(m_type, 16);
        return Render::QUniformValue::Invalid;
    }
}

} // of namespace Qt3D
