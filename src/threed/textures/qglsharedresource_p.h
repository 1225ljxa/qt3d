/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: http://www.qt-project.org/
**
** This file is part of the Qt3D module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QGLSHAREDRESOURCE_P_H
#define QGLSHAREDRESOURCE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QOpenGLBuffer>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

class QOpenGLContextManager;
class QOpenGLContextInfo;
class QOpenGLContext;

class QGLSharedResource
{
public:
    typedef void (*DestroyResourceFunc)(GLuint id);
    QGLSharedResource(DestroyResourceFunc destroyFunc)
        : m_destroyFunc(destroyFunc), m_contextInfo(0), m_id(0)
        , m_next(0), m_prev(0) {}
    ~QGLSharedResource() { destroy(); }

    QOpenGLContext *context() const;
    GLuint id() const { return m_id; }
    void clearId() { m_id = 0; }

    void attach(QOpenGLContext *context, GLuint id);
    void destroy();

private:
    DestroyResourceFunc m_destroyFunc;
    QOpenGLContextInfo *m_contextInfo;
    GLuint m_id;
    QGLSharedResource *m_next;
    QGLSharedResource *m_prev;

    friend class QOpenGLContextManager;
    friend class QOpenGLContextInfo;
};

QT_END_NAMESPACE

QT_END_HEADER

#endif
