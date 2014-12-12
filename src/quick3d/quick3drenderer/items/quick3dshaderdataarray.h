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

#ifndef QT3D_RENDER_QUICK_QUICK3DSHADERDATAARRAY_H
#define QT3D_RENDER_QUICK_QUICK3DSHADERDATAARRAY_H

#include <Qt3DCore/qnode.h>
#include <Qt3DQuickRenderer/qt3dquickrenderer_global.h>
#include <Qt3DQuickRenderer/quick3dshaderdata.h>
#include <QQmlListProperty>

QT_BEGIN_NAMESPACE

namespace Qt3D {

namespace Render {

namespace Quick {

class Quick3DShaderDataArrayPrivate;
class Quick3DShaderData;

class QT3DQUICKRENDERERSHARED_EXPORT Quick3DShaderDataArray : public QNode
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Qt3D::Render::Quick::Quick3DShaderData> values READ valuesList)
    Q_CLASSINFO("DefaultProperty", "values")
public:
    explicit Quick3DShaderDataArray(QNode *parent = 0);
    QQmlListProperty<Quick3DShaderData> valuesList();
    QList<Quick3DShaderData *> values() const;

protected:
    void copy(const QNode *ref) Q_DECL_OVERRIDE;

private:
    static void appendValue(QQmlListProperty<Quick3DShaderData> *list, Quick3DShaderData *bar);
    static Quick3DShaderData *valueAt(QQmlListProperty<Quick3DShaderData> *list, int index);
    static int valueCount(QQmlListProperty<Quick3DShaderData> *list);
    static void clearValues(QQmlListProperty<Quick3DShaderData> *list);
    Q_DECLARE_PRIVATE(Quick3DShaderDataArray)
    QT3D_CLONEABLE(Quick3DShaderDataArray)
};

} // Quick

} // Render

} // Qt3D

QT_END_NAMESPACE

Q_DECLARE_METATYPE(Qt3D::Render::Quick::Quick3DShaderDataArray *)

#endif // QT3D_RENDER_QUICK_QUICK3DSHADERDATAARRAY_H
