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

#ifndef QT3D_RENDER_QUICK_QUICK3DRENDERPASSFILTER_H
#define QT3D_RENDER_QUICK_QUICK3DRENDERPASSFILTER_H

#include <Qt3DQuick/quick3dnode.h>
#include <Qt3DRenderer/renderpassfilter.h>
#include <Qt3DRenderer/renderpasscriterion.h>
#include <Qt3DQuickRenderer/qt3dquickrenderer_global.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

namespace Render {

namespace Quick {

class QT3DQUICKRENDERERSHARED_EXPORT Quick3DRenderPassFilter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Qt3D::RenderPassCriterion> criteria READ criteriaList)
public:
    explicit Quick3DRenderPassFilter(QObject *parent = 0);

    QQmlListProperty<Qt3D::RenderPassCriterion> criteriaList();

    inline RenderPassFilter *parentRenderPassFilter() const { return qobject_cast<Qt3D::RenderPassFilter*>(parent()); }

private:
    static void appendCriterion(QQmlListProperty<RenderPassCriterion> *list, RenderPassCriterion *criterion);
    static RenderPassCriterion *criterionAt(QQmlListProperty<RenderPassCriterion> *list, int index);
    static int criteriaCount(QQmlListProperty<RenderPassCriterion> *list);
    static void clearCriteria(QQmlListProperty<RenderPassCriterion> *list);
};

} // Quick

} // Render

} // Qt3D

QT_END_NAMESPACE

#endif // QT3D_RENDER_QUICK_QUICK3DRENDERPASSFILTER_H
