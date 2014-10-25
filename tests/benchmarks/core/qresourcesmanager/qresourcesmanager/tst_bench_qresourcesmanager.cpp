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

#include <QtTest/QtTest>
#include <QMatrix4x4>
#include <Qt3DCore/qresourcesmanager.h>
#include <Qt3DCore/qhandle.h>
#include <ctime>

class tst_QResourcesManager : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void benchmarkAllocateSmallResources();
    void benchmarkAccessSmallResources();
    void benchmarkLookupSmallResources();
    void benchmarRandomAccessSmallResources();
    void benchmarkRandomLookupSmallResources();
    void benchmarkReleaseSmallResources();
    void benchmarkAllocateBigResources();
    void benchmarkAccessBigResources();
    void benchmarRandomAccessBigResources();
    void benchmarkLookupBigResources();
    void benchmarkRandomLookupBigResources();
    void benchmarkReleaseBigResources();
};

class tst_SmallArrayResource
{
public:
    tst_SmallArrayResource() : m_value(0)
    {}

    int m_value;
};

class tst_BigArrayResource
{
public:
    QMatrix4x4 m_matrix;
};

template<typename Resource>
void benchmarkAllocateResources()
{
    Qt3D::QResourcesManager<Resource, int, 16> manager;

    volatile Resource *c;
    QBENCHMARK_ONCE {
        const int max = (1 << 16) - 1;
        for (int i = 0; i < max; i++)
            c = manager.getOrCreateResource(i);
    }
    Q_UNUSED(c);
}

template<typename Resource>
void benchmarkAccessResources()
{
    Qt3D::QResourcesManager<Resource, int, 16> manager;
    const int max = (1 << 16) - 1;
    QVector<Qt3D::QHandle<Resource> > handles(max);
    for (int i = 0; i < max; i++)
        handles[i] = manager.acquire();

    volatile Resource *c;
    QBENCHMARK {
        for (int i = 0; i < max; i++)
            c = manager.data(handles[i]);
    }
    Q_UNUSED(c);
}

template<typename Resource>
void benchmarkRandomAccessResource() {
    Qt3D::QResourcesManager<Resource, int, 16> manager;
    const int max = (1 << 16) - 1;
    QVector<Qt3D::QHandle<Resource> > handles(max);
    for (int i = 0; i < max; i++)
        handles[i] = manager.acquire();

    std::srand(std::time(0));
    std::random_shuffle(handles.begin(), handles.end());
    volatile Resource *c;
    QBENCHMARK {
        for (int i = 0; i < max; i++)
            c = manager.data(handles[i]);
    }
    Q_UNUSED(c);
}

template<typename Resource>
void benchmarkLookupResources()
{
    Qt3D::QResourcesManager<Resource, int, 16> manager;
    const int max = (1 << 16) - 1;
    for (int i = 0; i < max; i++)
        manager.getOrCreateResource(i);

    volatile Resource *c;
    QBENCHMARK {
        for (int i = 0; i < max; i++)
            c = manager.lookupResource(i);
    }
    Q_UNUSED(c);
}

template<typename Resource>
void benchmarkRandomLookupResources()
{
    Qt3D::QResourcesManager<Resource, int, 16> manager;
    const int max = (1 << 16) - 1;
    QVector<int> resourcesIndices(max);
    for (int i = 0; i < max; i++) {
        manager.getOrCreateResource(i);
        resourcesIndices[i] = i;
    }
    std::srand(std::time(0));
    std::random_shuffle(resourcesIndices.begin(), resourcesIndices.end());
    volatile Resource *c;
    QBENCHMARK {
        for (int i = 0; i < max; i++)
            c = manager.lookupResource(resourcesIndices[i]);
    }
    Q_UNUSED(c);
}

template<typename Resource>
void benchmarkReleaseResources()
{
    Qt3D::QResourcesManager<Resource, int, 16> manager;
    const int max = (1 << 16) - 1;
    QVector<Qt3D::QHandle<Resource> > handles(max);
    for (int i = 0; i < max; i++)
        handles[i] = manager.acquire();

    QBENCHMARK_ONCE {
        manager.reset();
    }
}

void tst_QResourcesManager::benchmarkAllocateSmallResources()
{
    benchmarkAllocateResources<tst_SmallArrayResource>();
}

void tst_QResourcesManager::benchmarkAccessSmallResources()
{
    benchmarkAccessResources<tst_SmallArrayResource>();
}

void tst_QResourcesManager::benchmarkLookupSmallResources()
{
    benchmarkLookupResources<tst_SmallArrayResource>();
}

void tst_QResourcesManager::benchmarRandomAccessSmallResources()
{
    benchmarkRandomAccessResource<tst_SmallArrayResource>();
}

void tst_QResourcesManager::benchmarkRandomLookupSmallResources()
{
    benchmarkRandomLookupResources<tst_SmallArrayResource>();
}

void tst_QResourcesManager::benchmarkReleaseSmallResources()
{
    benchmarkReleaseResources<tst_SmallArrayResource>();
}

void tst_QResourcesManager::benchmarkAllocateBigResources()
{
    benchmarkAllocateResources<tst_BigArrayResource>();
}

void tst_QResourcesManager::benchmarkAccessBigResources()
{
    benchmarkAccessResources<tst_BigArrayResource>();
}

void tst_QResourcesManager::benchmarRandomAccessBigResources()
{
    benchmarkRandomAccessResource<tst_BigArrayResource>();
}

void tst_QResourcesManager::benchmarkLookupBigResources()
{
    benchmarkLookupResources<tst_BigArrayResource>();
}

void tst_QResourcesManager::benchmarkRandomLookupBigResources()
{
    benchmarkRandomLookupResources<tst_BigArrayResource>();
}

void tst_QResourcesManager::benchmarkReleaseBigResources()
{
    benchmarkReleaseResources<tst_BigArrayResource>();
}

QTEST_APPLESS_MAIN(tst_QResourcesManager)

#include "tst_bench_qresourcesmanager.moc"
