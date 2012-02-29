/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
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
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qaiscene.h"
#include "qaimesh.h"
#include "qailoader.h"

#include "qglscenenode.h"

QT_BEGIN_NAMESPACE

/*!
    \internal
    \class QAiScene
    \brief The QAiScene class manages and contains an asset importer scene.
    The scene consists of a QGLSceneNode object which is the top-level node
    and references the geometry imported from a file.
*/

/*!
    \internal
    Construct a new QAiScene object using the data in the \a scene,
    and setting the given \a parent.

    The QAiScene object takes ownership of the \a file.
*/
QAiScene::QAiScene(const aiScene *scene, QAiSceneHandler *handler)
    : QGLAbstractScene(0)
{
    Q_ASSERT(handler);
    Q_ASSERT(scene);
    QAiLoader loader(scene, handler);
    m_root = loader.loadMeshes();
    m_root->setParent(this);
    m_animations = loader.loadAnimations();
}

/*!
    \internal
    \reimp
    Destroy this QAiScene, recovering all resources.
*/
QAiScene::~QAiScene()
{
    // nothing to do here
}

/*!
    \internal
    \reimp
*/
QList<QObject *> QAiScene::objects() const
{
    QList<QObject *> objs;
    if (!m_root)
        return objs;
    objs.append(m_root);
    QList<QGLSceneNode*> children = m_root->allChildren();
    QList<QGLSceneNode*>::const_iterator it = children.constBegin();
    for ( ; it != children.constEnd(); ++it)
        objs.append(*it);
    return objs;
}

/*!
    \internal
    \reimp
*/
QGLSceneNode *QAiScene::mainNode() const
{
    return m_root;
}

QList<QGLSceneAnimation *> QAiScene::animations() const
{
    return m_animations;
}

QT_END_NAMESPACE
