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

#ifndef TAG_H
#define TAG_H

#include <QDebug>

#include <Qt3DRenderer/qt3drenderer_global.h>
#include <Qt3DCore/qnode.h>

QT_BEGIN_NAMESPACE

namespace Qt3D {

class QT3DRENDERERSHARED_EXPORT Tag : public QNode
{
    Q_OBJECT

    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(QVariant value READ value WRITE setValue NOTIFY valueChanged)

public:
    Tag(QNode *parent = 0)
        : QNode(parent)
        , m_name()
        , m_value()
    {}

    void setName( const QString& name )
    {
        if (name != m_name) {
            m_name = name;
            emit nameChanged();
        }
    }

    QString name() const { return m_name; }

    void setValue( const QVariant& value )
    {
        if (value != m_value) {
            m_value = value;
            emit valueChanged();
        }
    }

    QVariant value() const { return m_value; }

    void copy(const QNode *ref) Q_DECL_OVERRIDE
    {
        QNode::copy(ref);
        const Tag *tag = qobject_cast<const Tag *>(ref);
        if (tag != Q_NULLPTR) {
            m_name = tag->name();
            m_value = tag->value();
        }
    }

Q_SIGNALS:
    void nameChanged();
    void valueChanged();

private:
    QString m_name;
    QVariant m_value;
    Tag *doClone(QNode *clonedParent) const Q_DECL_OVERRIDE { return new Tag(clonedParent); }
};

} // namespace Qt3D

QT_END_NAMESPACE

#endif // TAG_H
