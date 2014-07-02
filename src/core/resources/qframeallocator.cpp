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

/*!
 * \class QFrameAllocator
 *
 * \brief Provides a pool of memory chuncks to be used to allocate objects on a per frame basis.
 * The memory can be recycled by following frames by calling clear which won't deallocate any memory.
 *
 *
 */


#include "qframeallocator.h"
#include "qframeallocator_p.h"

QT_BEGIN_NAMESPACE

namespace Qt3D {

QFrameAllocatorPrivate::QFrameAllocatorPrivate(QFrameAllocator *qq)
    : q_ptr(qq)
{
}

QFrameAllocator::QFrameAllocator(uint maxObjectSize, uint alignment, uint pageSize)
    : d_ptr(new QFrameAllocatorPrivate(this))
{
    Q_D(QFrameAllocator);
    d->m_maxObjectSize = maxObjectSize;
    d->m_alignment = alignment;
    d->m_allocatorPoolSize = allocatorIndexFromSize(maxObjectSize);
    Q_ASSERT(alignment && pageSize);
    d->m_allocatorPool.resize(d->m_allocatorPoolSize);
    for (int i = 0; i < d->m_allocatorPoolSize; i++) {
        d->m_allocatorPool[i].init((i + 1) * d->m_alignment, pageSize);
    }
}

QFrameAllocator::~QFrameAllocator()
{
    Q_D(QFrameAllocator);
    for (int i = 0; i < d->m_allocatorPoolSize; i++) {
        d->m_allocatorPool[i].release();
    }
}

// Clear all memory chunks, allocated memory is not released
void QFrameAllocator::clear()
{
    Q_D(QFrameAllocator);
    for (int i = 0; i < d->m_allocatorPoolSize; i++)
        d->m_allocatorPool[i].clear();
}

// Trim excess memory used by chunks
void QFrameAllocator::trim()
{
    Q_D(QFrameAllocator);
    for (int i = 0; i < d->m_allocatorPoolSize; i++)
        d->m_allocatorPool[i].trim();
}

uint QFrameAllocator::maxObjectSize() const
{
    Q_D(const QFrameAllocator);
    return d->m_maxObjectSize;
}

void *QFrameAllocator::allocateAtChunk(uint allocatorIndex)
{
    Q_D(QFrameAllocator);
    return d->m_allocatorPool[allocatorIndex].allocate();
}

void QFrameAllocator::deallocateAtChunck(void *ptr, uint allocatorIndex)
{
    Q_D(QFrameAllocator);
    d->m_allocatorPool[allocatorIndex].deallocate(ptr);
}

uint QFrameAllocator::allocatorIndexFromSize(uint targetSize) const
{
    Q_D(const QFrameAllocator);
    return (targetSize + d->m_alignment - 1) / d->m_alignment;
}

int QFrameAllocator::allocatorPoolSize() const
{
    Q_D(const QFrameAllocator);
    return d->m_allocatorPoolSize;
}

uint QFrameAllocator::totalChunkCount() const
{
    Q_D(const QFrameAllocator);
    uint chunkCount = 0;
    for (int i = 0; i < d->m_allocatorPoolSize; i++)
        chunkCount += d->m_allocatorPool[i].chunkCount();
    return chunkCount;
}

QFixedFrameAllocator::QFixedFrameAllocator()
    : m_blockSize(0)
    , m_nbrBlock(0)
    , m_lastAllocatedChunck(Q_NULLPTR)
    , m_lastFreedChunck(Q_NULLPTR)
{
}

QFixedFrameAllocator::~QFixedFrameAllocator() {
    release();
}

void QFixedFrameAllocator::init(uint blockSize, uchar pageSize)
{
    m_blockSize = blockSize;
    m_nbrBlock = pageSize;
}

void *QFixedFrameAllocator::allocate()
{
    Q_ASSERT(m_blockSize && m_nbrBlock);
    if (m_lastAllocatedChunck == Q_NULLPTR ||
            m_lastAllocatedChunck->m_blocksAvailable == 0) {
        int i = 0;
        for (; i < m_chunks.size(); i++) {
            if (m_chunks[i].m_blocksAvailable > 0) {
                m_lastAllocatedChunck = m_chunks.begin() + i;
                break;
            }
        }
        if (i == m_chunks.size()) {
            m_chunks.resize(m_chunks.size() + 1);
            QFrameChunk &newChunk = m_chunks.last();
            newChunk.init(m_blockSize, m_nbrBlock);
            m_lastAllocatedChunck = &newChunk;
            m_lastFreedChunck = m_lastAllocatedChunck;
        }
    }
    return m_lastAllocatedChunck->allocate(m_blockSize);
}

void QFixedFrameAllocator::deallocate(void *ptr)
{
    Q_ASSERT(m_blockSize && m_nbrBlock);
    if (!m_chunks.empty() && ptr != Q_NULLPTR) {
        if (m_lastFreedChunck != Q_NULLPTR && m_lastFreedChunck->contains(ptr, m_blockSize))
            m_lastFreedChunck->deallocate(ptr, m_blockSize);
        else {
            for (int i = 0; i < m_chunks.size(); i++) {
                if (m_chunks[i].contains(ptr, m_blockSize)) {
                    m_chunks[i].deallocate(ptr, m_blockSize);
                    m_lastFreedChunck = m_chunks.begin() + i;
                    break ;
                }
            }
        }
    }
}

void QFixedFrameAllocator::trim()
{
    for (int i = m_chunks.size() - 1; i >= 0; i--) {
        if (m_chunks.at(i).isEmpty()) {
            m_chunks[i].release();
            if (m_lastAllocatedChunck == &m_chunks[i])
                m_lastAllocatedChunck = Q_NULLPTR;
            if (m_lastFreedChunck == &m_chunks[i])
                m_lastAllocatedChunck = Q_NULLPTR;
            m_chunks.removeAt(i);
        }
    }
}

void QFixedFrameAllocator::release()
{
    for (int i = m_chunks.size() - 1; i >= 0; i--)
        m_chunks[i].release();
    m_chunks.clear();
    m_lastAllocatedChunck = Q_NULLPTR;
    m_lastFreedChunck = Q_NULLPTR;
}

// Allows to reuse chuncks without having to reinitialize and reallocate them
void QFixedFrameAllocator::clear()
{
    for (int i = m_chunks.size() - 1; i >= 0; i--)
        m_chunks[i].clear(m_blockSize, m_nbrBlock);
}

// QFrameChuck is agnostic about blocksize
// However if it was initialized with a block size of 16
// You should then pass 16 to allocate and deallocate
void QFrameChunk::init(uint blockSize, uchar blocks)
{
    m_data = new uchar[blockSize * blocks];
    m_firstAvailableBlock = 0;
    m_blocksAvailable = blocks;
    m_maxBlocksAvailable = blocks;
    uchar *p = m_data;
    // Init each block with its position stored in its first byte
    for (uchar i = 0; i < blocks; p += blockSize)
        *p = ++i;
}

void *QFrameChunk::allocate(uint blockSize)
{
    if (m_blocksAvailable == 0)
        return Q_NULLPTR;
    uchar *r = m_data + (m_firstAvailableBlock * blockSize);
    m_firstAvailableBlock = *r;
    --m_blocksAvailable;
    return r;
}

// Shouldn't be called more than once for the same pointer
void QFrameChunk::deallocate(void *p, uint blockSize)
{
    if (p >= m_data) {
        uchar *toRelease = static_cast<uchar *>(p);
        uchar oldFreeBlock = m_firstAvailableBlock;
        m_firstAvailableBlock = static_cast<uchar>((toRelease - m_data) / blockSize);
        *toRelease = oldFreeBlock;
        ++m_blocksAvailable;
    }
}

bool QFrameChunk::contains(void *p, uint blockSize)
{
    uchar *c = static_cast<uchar *>(p);
    return (m_data <= c && c < m_data + blockSize * m_maxBlocksAvailable);
}

// Reset chunck without releasing heap allocated memory
void QFrameChunk::clear(uint blockSize, uchar blocks)
{
    m_firstAvailableBlock = 0;
    m_blocksAvailable = blocks;

    uchar *p = m_data;
    // Init each block with its position stored in its first byte
    for (uchar i = 0; i < blocks; p += blockSize)
        *p = ++i;
}

void QFrameChunk::release()
{
    delete [] m_data;
}

} // Qt3D

QT_END_NAMESPACE
