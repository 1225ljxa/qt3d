/****************************************************************************
**
** Copyright (C) 2015 Klaralvdalens Datakonsult AB (KDAB).
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

#include "qtextureimage.h"
#include "qabstracttextureimage_p.h"

QT_BEGIN_NAMESPACE

namespace Qt3D {

class QTextureImagePrivate: public QAbstractTextureImagePrivate
{
public:
    QTextureImagePrivate(QTextureImage *qq)
        : QAbstractTextureImagePrivate(qq)
    {
    }

    Q_DECLARE_PUBLIC(QTextureImage)
    QUrl m_source;
};

class QImageTextureDataFunctor : public QTextureDataFunctor
{
public:
    QImageTextureDataFunctor(const QUrl &url)
        : QTextureDataFunctor()
        , m_url(url)
    {}

    // Will be executed from within a QAspectJob
    TexImageDataPtr operator ()() Q_DECL_FINAL
    {
        TexImageDataPtr dataPtr;
        if (m_url.isLocalFile() || m_url.scheme() == QStringLiteral("qrc")) {
            QString source = m_url.toString().replace(QStringLiteral("qrc"), QStringLiteral(""));
            QImage img;
            if (img.load(source)) {
                dataPtr.reset(new TexImageData());
                dataPtr->setImage(img);
            } else {
                qWarning() << "Failed to load image : " << source;
            }
        } else {
            qWarning() << "implement loading from remote URLs";
        }
        return dataPtr;
    }

    bool operator ==(const QTextureDataFunctor &other) const Q_DECL_FINAL
    {
        const QImageTextureDataFunctor *otherFunctor = dynamic_cast<const QImageTextureDataFunctor*>(&other);
        return (otherFunctor != Q_NULLPTR && otherFunctor->m_url == m_url);
    }

private:
    QUrl m_url;
};

/*!
    \class Qt3D::QTextureImage
    \since 5.5
    \brief Encapsulates the necessary information to create an OpenGL texture
    image from an image source.

    It contains the necessary information mipmap level, layer, cube face and
    source URL to load at the proper place data into an OpenGL texture.
 */

/*!
    Constructs a new Qt3D::QTextureImage instance with \a parent as parent.
 */
QTextureImage::QTextureImage(QNode *parent)
    : QAbstractTextureImage(*new QTextureImagePrivate(this), parent)
{
}

QTextureImage::~QTextureImage()
{
}

/*!
    \return the source url from which data will be loaded of the texture image.
 */
QUrl QTextureImage::source() const
{
    Q_D(const QTextureImage);
    return d->m_source;
}

/*!
    Sets the source url of the texture image to \a source.
    \note: this triggers a call to update()
 */
void QTextureImage::setSource(const QUrl &source)
{
    Q_D(QTextureImage);
    if (source != d->m_source) {
        d->m_source = source;
        emit sourceChanged();
        update();
    }
}

/*!
    \returns the a Qt3D::QTextureDataFunctorPtr functor to be used by the
    backend to load the texture image data into an OpenGL texture object.
 */
QTextureDataFunctorPtr QTextureImage::dataFunctor() const
{
    return QTextureDataFunctorPtr(new QImageTextureDataFunctor(source()));
}

void QTextureImage::copy(const QNode *ref)
{
    QAbstractTextureImage::copy(ref);
    const QTextureImage *img = static_cast<const QTextureImage *>(ref);
    d_func()->m_source = img->source();
}

} // Qt3D

QT_END_NAMESPACE

