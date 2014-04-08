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

#ifndef QT3D_QHANDLEMANAGER_H
#define QT3D_QHANDLEMANAGER_H

#include <QtGlobal>
#include <Qt3DCore/qt3dcore_global.h>
#include <Qt3DCore/qhandle.h>

#include <QtCore/QVector>

QT_BEGIN_NAMESPACE

namespace Qt3D {

template <typename T, int INDEXBITS = 16>
class QHandleManager
{
public:
    QHandleManager()
        : m_firstFreeEntry(0)
        , m_activeEntryCount(0)
        , m_entries(1 << INDEXBITS)
    {
        reset();
    }

    quint32 activeEntries() const { return m_activeEntryCount; }

    void reset();
    QHandle<T> acquire(T *data);
    void release(const QHandle<T> &handle);
    void update(const QHandle<T> &, T *data);
    T *data(const QHandle<T> &handle, bool *ok = 0);
    const T *constData(const QHandle<T> &handle, bool *ok = 0) const;

private:
    Q_DISABLE_COPY(QHandleManager)

    template <typename U>
    struct HandleEntry
    {
        HandleEntry()
            : m_data(Q_NULLPTR)
            , m_nextFreeIndex(0)
            , m_counter(0)
            , m_active(false)
            , m_endOfFreeList(false)
        {}

        explicit HandleEntry(quint32 nextFreeIndex)
            : m_data(Q_NULLPTR)
            , m_nextFreeIndex(nextFreeIndex)
            , m_counter(0)
            , m_active(false)
            , m_endOfFreeList(false)
        {}

        U *m_data;
        unsigned int m_nextFreeIndex : QHandle<U>::IndexBits;
        unsigned int m_counter : QHandle<U>::CounterBits;
        unsigned int m_active : 1;
        unsigned int m_endOfFreeList : 1;
    };

    quint32 m_firstFreeEntry;
    int m_activeEntryCount;
    QVector<HandleEntry<T> > m_entries;
};

template <typename T, int INDEXBITS>
void QHandleManager<T, INDEXBITS>::reset()
{
    m_activeEntryCount = 0;
    m_firstFreeEntry = 0;

    for (int i = 0; i < QHandle<T>::MaxIndex; ++i)
        m_entries[i] = HandleEntry<T>(i + 1);
    m_entries[QHandle<T>::MaxIndex] = HandleEntry<T>();
    m_entries[QHandle<T>::MaxIndex].m_endOfFreeList = true;
}

template <typename T, int INDEXBITS>
QHandle<T> QHandleManager<T, INDEXBITS>::acquire(T *data)
{
    Q_ASSERT(m_activeEntryCount < QHandle<T>::MaxIndex);

    const quint32 newIndex = m_firstFreeEntry;
    Q_ASSERT(newIndex < QHandle<T>::MaxIndex);
    Q_ASSERT(m_entries[newIndex].m_active == false);
    Q_ASSERT(!m_entries[newIndex].m_endOfFreeList);

    m_firstFreeEntry = m_entries[newIndex].m_nextFreeIndex;
    m_entries[newIndex].m_nextFreeIndex = 0;
    ++m_entries[newIndex].m_counter;
    if (m_entries[newIndex].m_counter == 0)
        m_entries[newIndex].m_counter = 1;
    m_entries[newIndex].m_active = true;
    m_entries[newIndex].m_data = data;

    ++m_activeEntryCount;

    return QHandle<T>(newIndex, m_entries[newIndex].m_counter);
}

template <typename T, int INDEXBITS>
void QHandleManager<T, INDEXBITS>::release(const QHandle<T> &handle)
{
    const quint32 index = handle.index();
    Q_ASSERT(m_entries[index].m_counter == handle.counter());
    Q_ASSERT(m_entries[index].m_active == true);

    m_entries[index].m_nextFreeIndex = m_firstFreeEntry;
    m_entries[index].m_active = false;
    m_firstFreeEntry = index;

    --m_activeEntryCount;
}

// Needed in case the QResourcesManager has reordered
// memory so that the handle still points to valid data
template <typename T, int INDEXBITS>
void QHandleManager<T, INDEXBITS>::update(const QHandle<T> &handle, T *data)
{
    const quint32 index = handle.index();
    Q_ASSERT(m_entries[index].m_counter == handle.counter());
    Q_ASSERT(m_entries[index].m_active == true);
    m_entries[index].m_data = data;
}

template <typename T, int INDEXBITS>
T *QHandleManager<T, INDEXBITS>::data(const QHandle<T> &handle, bool *ok)
{
    const quint32 index = handle.index();
    if (m_entries[index].m_counter != handle.counter() ||
        m_entries[index].m_active == false) {
        if (ok)
            *ok = false;
        return Q_NULLPTR;
    }

    T *data = m_entries[index].m_data;
    if (ok)
        *ok = true;
    return data;
}

template <typename T, int INDEXBITS>
const T *QHandleManager<T, INDEXBITS>::constData(const QHandle<T> &handle, bool *ok) const
{
    const quint32 index = handle.index();
    if (m_entries[index].m_counter != handle.counter() ||
        m_entries[index].m_active == false) {
        if (ok)
            *ok = false;
        return Q_NULLPTR;
    }

    const T *data = m_entries[index].m_data;
    if (ok)
        *ok = true;
    return data;
}

} // Qt3D

QT_END_NAMESPACE

#endif // QT3D_QHANDLEMANAGER_H
