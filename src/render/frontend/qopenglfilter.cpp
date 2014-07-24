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

#include "qopenglfilter.h"
#include "private/qobject_p.h"

QT_BEGIN_NAMESPACE

namespace Qt3D {

class QOpenGLFilterPrivate : public QObjectPrivate
{
public:

    QOpenGLFilterPrivate(QOpenGLFilter *qq)
        : QObjectPrivate()
        , m_minor(0)
        , m_major(0)
    {
        q_ptr = qq;
    }

    Q_DECLARE_PUBLIC(QOpenGLFilter)
    QOpenGLFilter::Api m_api;
    QOpenGLFilter::Profile m_profile;
    int m_minor;
    int m_major;
    QStringList m_extensions;
    QString m_vendor;
};

QOpenGLFilter::QOpenGLFilter(QObject *parent)
    : QObject(*new QOpenGLFilterPrivate(this), parent)
{
}

QOpenGLFilter::Api QOpenGLFilter::api() const
{
    Q_D(const QOpenGLFilter);
    return d->m_api;
}

QOpenGLFilter::Profile QOpenGLFilter::profile() const
{
    Q_D(const QOpenGLFilter);
    return d->m_profile;
}

int QOpenGLFilter::minorVersion() const
{
    Q_D(const QOpenGLFilter);
    return d->m_minor;
}

int QOpenGLFilter::majorVersion() const
{
    Q_D(const QOpenGLFilter);
    return d->m_major;
}

QStringList QOpenGLFilter::extensions() const
{
    Q_D(const QOpenGLFilter);
    return d->m_extensions;
}

QString QOpenGLFilter::vendor() const
{
    Q_D(const QOpenGLFilter);
    return d->m_vendor;
}

void QOpenGLFilter::setApi(QOpenGLFilter::Api api)
{
    Q_D(QOpenGLFilter);
    if (d->m_api != api) {
        d->m_api = api;
        emit apiChanged();
    }
}

void QOpenGLFilter::setProfile(QOpenGLFilter::Profile profile)
{
    Q_D(QOpenGLFilter);
    if (d->m_profile != profile) {
        d->m_profile = profile;
        emit profileChanged();
    }
}

void QOpenGLFilter::setMinorVersion(int minorVersion)
{
    Q_D(QOpenGLFilter);
    if (minorVersion != d->m_minor) {
        d->m_minor = minorVersion;
        emit minorVersionChanged();
    }
}

void QOpenGLFilter::setMajorVersion(int majorVersion)
{
    Q_D(QOpenGLFilter);
    if (d->m_major != majorVersion) {
        d->m_major = majorVersion;
        emit majorVersionChanged();
    }
}

void QOpenGLFilter::setExtensions(const QStringList &extensions)
{
    Q_D(QOpenGLFilter);
    if (d->m_extensions != extensions) {
        d->m_extensions = extensions;
        emit extensionsChanged();
    }
}

void QOpenGLFilter::setVendor(const QString &vendor)
{
    Q_D(QOpenGLFilter);
    if (d->m_vendor != vendor) {
        d->m_vendor = vendor;
        emit vendorChanged();
    }
}

} // Qt3D

QT_END_NAMESPACE
