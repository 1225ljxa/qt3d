/****************************************************************************
**
** Copyright (C) 2015 Klaralvdalens Datakonsult AB (KDAB).
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

#ifndef QT3DRENDER_QTEXTUREPROVIDERS_H
#define QT3DRENDER_QTEXTUREPROVIDERS_H

#include <QUrl>
#include <Qt3DRender/qabstracttextureprovider.h>

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

class QT3DRENDERSHARED_EXPORT QTexture1D : public QAbstractTextureProvider
{
    Q_OBJECT
public:
    explicit QTexture1D(Qt3DCore::QNode *parent = 0);
    ~QTexture1D();
};

class QT3DRENDERSHARED_EXPORT QTexture1DArray : public QAbstractTextureProvider
{
    Q_OBJECT
public:
    explicit QTexture1DArray(Qt3DCore::QNode *parent = 0);
    ~QTexture1DArray();
};

class QT3DRENDERSHARED_EXPORT QTexture2D : public QAbstractTextureProvider
{
    Q_OBJECT
public:
    explicit QTexture2D(Qt3DCore::QNode *parent = 0);
    ~QTexture2D();
};

class QT3DRENDERSHARED_EXPORT QTexture2DArray : public QAbstractTextureProvider
{
    Q_OBJECT
public:
    explicit QTexture2DArray(Qt3DCore::QNode *parent = 0);
    ~QTexture2DArray();
};

class QT3DRENDERSHARED_EXPORT QTexture3D : public QAbstractTextureProvider
{
    Q_OBJECT
public:
    explicit QTexture3D(Qt3DCore::QNode *parent = 0);
    ~QTexture3D();
};

class QT3DRENDERSHARED_EXPORT QTextureCubeMap : public QAbstractTextureProvider
{
    Q_OBJECT
public:
    explicit QTextureCubeMap(Qt3DCore::QNode *parent = 0);
    ~QTextureCubeMap();
};

class QT3DRENDERSHARED_EXPORT QTextureCubeMapArray : public QAbstractTextureProvider
{
    Q_OBJECT
public:
    explicit QTextureCubeMapArray(Qt3DCore::QNode *parent = 0);
    ~QTextureCubeMapArray();
};

class QT3DRENDERSHARED_EXPORT QTexture2DMultisample : public QAbstractTextureProvider
{
    Q_OBJECT
public:
    explicit QTexture2DMultisample(Qt3DCore::QNode *parent = 0);
    ~QTexture2DMultisample();
};

class QT3DRENDERSHARED_EXPORT QTexture2DMultisampleArray : public QAbstractTextureProvider
{
    Q_OBJECT
public:
    explicit QTexture2DMultisampleArray(Qt3DCore::QNode *parent = 0);
    ~QTexture2DMultisampleArray();
};

class QT3DRENDERSHARED_EXPORT QTextureRectangle : public QAbstractTextureProvider
{
    Q_OBJECT
public:
    explicit QTextureRectangle(Qt3DCore::QNode *parent = 0);
    ~QTextureRectangle();
};

class QT3DRENDERSHARED_EXPORT QTextureBuffer : public QAbstractTextureProvider
{
    Q_OBJECT
public:
    explicit QTextureBuffer(Qt3DCore::QNode *parent = 0);
    ~QTextureBuffer();
};

class QTextureLoaderPrivate;

class QT3DRENDERSHARED_EXPORT QTextureLoader : public QAbstractTextureProvider
{
    Q_OBJECT
    Q_PROPERTY(QUrl source READ source WRITE setSource NOTIFY sourceChanged)

public:
    explicit QTextureLoader(Qt3DCore::QNode *parent = 0);
    ~QTextureLoader();

    QUrl source() const;

public Q_SLOTS:
    void setSource(const QUrl &source);

Q_SIGNALS:
    void sourceChanged(const QUrl &source);

private:
    Q_DECLARE_PRIVATE(QTextureLoader)
};

} // namespace Qt3DRender

QT_END_NAMESPACE

#endif // QT3DRENDER_QTEXTUREPROVIDERS_H
