/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL3$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPLv3 included in the
** packaging of this file. Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: https://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 2.0 or later as published by the Free
** Software Foundation and appearing in the file LICENSE.GPL included in
** the packaging of this file. Please review the following information to
** ensure the GNU General Public License version 2.0 requirements will be
** met: http://www.gnu.org/licenses/gpl-2.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <iterator>

#include "dependencyhandler_p.h"

QT_BEGIN_NAMESPACE

namespace Qt3D {

namespace {
    template <template <typename T> class Op>
    struct ByDepender {
        typedef bool result_type;

        bool operator()(const QSharedPointer<TaskInterface> &lhs, const QSharedPointer<TaskInterface> &rhs) const Q_DECL_NOTHROW
        { return Op<QSharedPointer<TaskInterface> >()(lhs, rhs); }

        bool operator()(const QSharedPointer<TaskInterface> &lhs, const Dependency &rhs) const Q_DECL_NOTHROW
        { return operator()(lhs, rhs.depender); }

        bool operator()(const Dependency &lhs, const QSharedPointer<TaskInterface> &rhs) const Q_DECL_NOTHROW
        { return operator()(lhs.depender, rhs); }

        bool operator()(const Dependency &lhs, const Dependency &rhs) const Q_DECL_NOTHROW
        { return operator()(lhs.depender, rhs.depender); }
    };

    struct DependeeEquals : std::unary_function<Dependency, bool>
    {
        QSharedPointer<TaskInterface> dependee;
        explicit DependeeEquals(QSharedPointer<TaskInterface> dependee)
            : dependee(qMove(dependee)) {}
        bool operator()(const Dependency &candidate) const
        {
            return dependee == candidate.dependee;
        }
    };

    struct ByDependerThenDependee : std::binary_function<Dependency, Dependency, bool>
    {
        // Defines a lexicographical order (depender first).
        bool operator()(const Dependency &lhs, const Dependency &rhs)
        {
            if (lhs.depender < rhs.depender) return true;
            if (rhs.depender < lhs.depender) return false;
            return lhs.dependee < rhs.dependee;
        }
    };
}

DependencyHandler::DependencyHandler()
{
}

void DependencyHandler::addDependencies(QVector<Dependency> dependencies)
{
    std::sort(dependencies.begin(), dependencies.end(), ByDependerThenDependee());

    const QMutexLocker locker(&m_mutex);

    QVector<Dependency> newDependencyMap;
    newDependencyMap.reserve(dependencies.size() + m_dependencyMap.size());
    std::set_union(m_dependencyMap.begin(), m_dependencyMap.end(),
                   dependencies.begin(), dependencies.end(),
                   std::back_inserter(newDependencyMap), ByDependerThenDependee());
    m_dependencyMap.swap(newDependencyMap); // commit
}

bool DependencyHandler::hasDependency(const QSharedPointer<TaskInterface> &depender)
{
    const QMutexLocker locker(&m_mutex);

    return std::binary_search(m_dependencyMap.begin(), m_dependencyMap.end(),
                              depender, ByDepender<std::less>());
}

/*
 * Removes all the entries on the m_dependencyMap that have given task as a dependee,
 * i.e. entries where the dependency is on the given task.
 */
void DependencyHandler::freeDependencies(const QSharedPointer<TaskInterface> &dependee)
{
    const QMutexLocker locker(&m_mutex);

    m_dependencyMap.erase(std::remove_if(m_dependencyMap.begin(), m_dependencyMap.end(),
                                         DependeeEquals(dependee)),
                          m_dependencyMap.end());
}

} // namespace Qt3D

QT_END_NAMESPACE
