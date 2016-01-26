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

#include "qtextureimage.h"
#include "qtextureimage_p.h"
#include "qabstracttextureimage_p.h"

QT_BEGIN_NAMESPACE

namespace Qt3DRender {

class QTextureImagePrivate : public QAbstractTextureImagePrivate
{
public:
    QTextureImagePrivate()
        : QAbstractTextureImagePrivate()
    {
    }

    Q_DECLARE_PUBLIC(QTextureImage)
    QUrl m_source;
};

/*!
    \class Qt3DRender::QTextureImage
    \inmodule Qt3DRender
    \since 5.5
    \brief Encapsulates the necessary information to create an OpenGL texture
    image from an image source.

    It contains the necessary information mipmap level, layer, cube face and
    source URL to load at the proper place data into an OpenGL texture.
 */

/*!
    \qmltype TextureImage
    \instantiates Qt3DRender::QTextureImage
    \inherits AbstractTextureImage
    \inqmlmodule Qt3D.Render
    \since 5.5
    \brief Encapsulates the necessary information to create an OpenGL texture
    image from an image source.
*/

/*!
    Constructs a new Qt3DRender::QTextureImage instance with \a parent as parent.
 */
QTextureImage::QTextureImage(QNode *parent)
    : QAbstractTextureImage(*new QTextureImagePrivate, parent)
{
}

/*!
  The destructor.
 */
QTextureImage::~QTextureImage()
{
    QNode::cleanup();
}

/*!
    Returns the source url from which data for the texture image will be loaded.
 */
QUrl QTextureImage::source() const
{
    Q_D(const QTextureImage);
    return d->m_source;
}

/*!
  \property Qt3DRender::QTextureImage::source

  This property holdsthe source url from which data for the texture
  image will be loaded.
*/

/*!
  \qmlproperty url Qt3D.Render::TextureImage::source

  This property holdsthe source url from which data for the texture
  image will be loaded.
*/

/*!
    Sets the source url of the texture image to \a source.
    \note This triggers a call to update()
 */
void QTextureImage::setSource(const QUrl &source)
{
    Q_D(QTextureImage);
    if (source != d->m_source) {
        d->m_source = source;
        emit sourceChanged(source);
        update();
    }
}

/*!
    Returns the Qt3DRender::QTextureDataFunctorPtr functor to be used by the
    backend to load the texture image data into an OpenGL texture object.
 */
QTextureDataFunctorPtr QTextureImage::dataFunctor() const
{
    return QTextureDataFunctorPtr(new QImageTextureDataFunctor(source()));
}

/*!
  Copies \a ref into this texture image.
 */
void QTextureImage::copy(const QNode *ref)
{
    QAbstractTextureImage::copy(ref);
    const QTextureImage *img = static_cast<const QTextureImage *>(ref);
    d_func()->m_source = img->source();
}

} // namespace Qt3DRender

QT_END_NAMESPACE

