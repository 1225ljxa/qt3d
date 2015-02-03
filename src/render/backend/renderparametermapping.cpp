/****************************************************************************
**
** Copyright (C) 2015 Klaralvdalens Datakonsult AB (KDAB).
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

#include "renderparametermapping_p.h"

QT_BEGIN_NAMESPACE

namespace Qt3D {

namespace Render {

RenderParameterMapping::RenderParameterMapping()
    : m_bindingType(QParameterMapping::Uniform)
{
}

RenderParameterMapping::RenderParameterMapping(QParameterMapping *mapping)
    : m_id(mapping ? mapping->id() : QNodeId())
    , m_parameterName(mapping ? mapping->parameterName() : QString())
    , m_shaderVariableName(mapping ? mapping->shaderVariableName() : QString())
    , m_bindingType(mapping ? mapping->bindingType() : QParameterMapping::Uniform)
{
}

bool RenderParameterMapping::isValid() const
{
    return !m_id.isNull();
}

QNodeId RenderParameterMapping::id() const
{
    return m_id;
}

QString RenderParameterMapping::parameterName() const
{
    return m_parameterName;
}

QString RenderParameterMapping::shaderVariableName() const
{
    return m_shaderVariableName;
}

QParameterMapping::Binding RenderParameterMapping::bindingType() const
{
    return m_bindingType;
}

} // Render

} // Qt3D

QT_END_NAMESPACE
