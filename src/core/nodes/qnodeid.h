/****************************************************************************
**
** Copyright (C) 2014 Klaralvdalens Datakonsult AB (KDAB).
** Contact: https://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl-3.0.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or (at your option) the GNU General
** Public license version 3 or any later version approved by the KDE Free
** Qt Foundation. The licenses are as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL2 and LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-2.0.html and
** https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QT3DCORE_QNODEID_H
#define QT3DCORE_QNODEID_H

#include <Qt3DCore/qt3dcore_global.h>
#include <QDebug>

QT_BEGIN_NAMESPACE

namespace Qt3DCore {

class QT3DCORESHARED_EXPORT QNodeId
{
public:
    QNodeId()
        : m_id(0)
    {}

    static QNodeId createId();

    inline bool isNull() const
    {
        return m_id == 0;
    }

    inline bool operator ==(const QNodeId &other) const
    {
        return other.m_id == m_id;
    }

    inline bool operator !=(const QNodeId &other) const
    {
        return !operator ==(other);
    }

    inline bool operator <(const QNodeId &other) const
    {
        return m_id < other.m_id;
    }

    inline bool operator >(const QNodeId &other) const
    {
        return m_id > other.m_id;
    }

    inline quint64 id() const
    {
        return m_id;
    }

private:
    quint64 m_id;
};


#ifndef QT_NO_DEBUG_STREAM
QT3DCORESHARED_EXPORT QDebug operator<<(QDebug d, const QNodeId &id);
#endif

inline uint qHash(const QNodeId &id, uint seed = 0) Q_DECL_NOTHROW
{
    Q_UNUSED(seed);
    return id.id();
}

} // Qt3D

Q_DECLARE_TYPEINFO(Qt3DCore::QNodeId, Q_MOVABLE_TYPE);

QT_END_NAMESPACE

Q_DECLARE_METATYPE(Qt3DCore::QNodeId)


#endif // QT3DCORE_QNODEID_H
