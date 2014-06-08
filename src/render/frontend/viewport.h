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

#ifndef QT3D_VIEWPORT_H
#define QT3D_VIEWPORT_H

#include <Qt3DRenderer/framegraphitem.h>
#include <Qt3DCore/node.h>
#include <Qt3DCore/qscenepropertychange.h>
#include <QtCore/QRectF>

QT_BEGIN_NAMESPACE

namespace Qt3D {

class Viewport;

class ViewportPrivate
{
public :
    ViewportPrivate(Viewport *qq) :
        q_ptr(qq)
    {}

    Q_DECLARE_PUBLIC(Viewport)
    Viewport *q_ptr;
    QRectF m_rect;
};

class QT3DRENDERERSHARED_EXPORT Viewport : public Node, public FrameGraphItem
{
    Q_OBJECT
    Q_INTERFACES(Qt3D::FrameGraphItem)
    Q_PROPERTY(QRectF rect READ rect WRITE setRect NOTIFY rectChanged)

public:
    explicit Viewport(Node *parent = 0)
        : Node(parent)
        , d_ptr(new ViewportPrivate(this))
    {}

    virtual ~Viewport() {}

    QRectF rect() const
    {
        Q_D(const Viewport);
        return d->m_rect;
    }

    void setRect(const QRectF& rect)
    {
        Q_D(Viewport);
        if (rect != d->m_rect) {
            d->m_rect = rect;
            emit rectChanged();
            QScenePropertyChangePtr propertyChange(new QScenePropertyChange(ComponentUpdated, this));
            propertyChange->m_propertyName = QByteArrayLiteral("rect");
            propertyChange->m_value = QVariant::fromValue(d->m_rect);
            notifyObservers(propertyChange);
        }
    }

Q_SIGNALS:
    void rectChanged();
    void enabledChanged() Q_DECL_OVERRIDE;


private:
    Q_DECLARE_PRIVATE(Viewport)
    ViewportPrivate *d_ptr;
};

} // Qt3D

QT_END_NAMESPACE

#endif // QT3D_VIEWPORT_H
