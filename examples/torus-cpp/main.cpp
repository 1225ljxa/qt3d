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

#include <QGuiApplication>

#include <Qt3DCore/window.h>
#include <Qt3DCore/camera.h>
#include <Qt3DCore/qentity.h>
#include <Qt3DCore/cameralens.h>

#include <Qt3DRenderer/qtorusmesh.h>
#include <Qt3DRenderer/qmesh.h>
#include <Qt3DRenderer/qtechnique.h>
#include <Qt3DRenderer/qmaterial.h>
#include <Qt3DCore/qabstracteffect.h>
#include <Qt3DRenderer/texture.h>
#include <Qt3DRenderer/qrenderpass.h>
#include <Qt3DRenderer/qscene.h>

#include <Qt3DCore/qscaletransform.h>
#include <Qt3DCore/qrotatetransform.h>
#include <Qt3DCore/qlookattransform.h>
#include <Qt3DCore/qtransform.h>

#include <Qt3DRenderer/qcameraselector.h>
#include <Qt3DRenderer/qrenderpassfilter.h>
#include <Qt3DRenderer/qtechniquefilter.h>
#include <Qt3DRenderer/qviewport.h>
#include <Qt3DRenderer/rendereraspect.h>
#include <Qt3DRenderer/qframegraph.h>

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    Qt3D::Window view;
    view.registerAspect(new Qt3D::RendererAspect());

    // Root entity
    Qt3D::QEntity *rootEntity = new Qt3D::QEntity();

    // Camera
    Qt3D::Camera *cameraEntity = new Qt3D::Camera();
    cameraEntity->setObjectName(QStringLiteral("cameraEntity"));
    Qt3D::CameraLens *cameraLens = new Qt3D::CameraLens();
    Qt3D::QTransform *cameraTransform = new Qt3D::QTransform();
    Qt3D::QLookAtTransform *cameraLookAtTransform = new Qt3D::QLookAtTransform();

    cameraLens->setPerspectiveProjection(45.0f, 16.0f/9.0f, 0.1f, 1000.0f);
    cameraLookAtTransform->setPosition(QVector3D(0, 0, -20.0f));
    cameraLookAtTransform->setUpVector(QVector3D(0, 1, 0));
    cameraLookAtTransform->setViewCenter(QVector3D(0, 0, 0));
    cameraTransform->appendTransform(cameraLookAtTransform);
    cameraEntity->setTransform(cameraTransform);
    cameraEntity->setLens(cameraLens);
    view.setCamera(cameraEntity);

    // FrameGraph
    Qt3D::QFrameGraph *frameGraph = new Qt3D::QFrameGraph();
    Qt3D::QTechniqueFilter *techniqueFilter = new Qt3D::QTechniqueFilter();
    Qt3D::QCameraSelector *cameraSelector = new Qt3D::QCameraSelector();
    Qt3D::QRenderPassFilter *defaultRenderPassFilter = new Qt3D::QRenderPassFilter();
    Qt3D::QViewport *viewport = new Qt3D::QViewport();

    // TechiqueFilter and renderPassFilter are not implement yet

    viewport->setRect(QRectF(0, 0, 1, 1));
    viewport->addChild(cameraSelector);
    cameraSelector->setCamera(cameraEntity);
    cameraSelector->addChild(defaultRenderPassFilter);
    techniqueFilter->addChild(viewport);
    frameGraph->setActiveFrameGraph(techniqueFilter);


    // Torus shape data
    Qt3D::QTorusMesh *torus = new Qt3D::QTorusMesh();
    torus->setRadius(5);
    torus->setMinorRadius(1);
    torus->setRings(100);
    torus->setSlices(20);

    // TorusMesh Transform
    Qt3D::QScaleTransform *torusScale = new Qt3D::QScaleTransform();
    Qt3D::QRotateTransform *torusRotation = new Qt3D::QRotateTransform();
    Qt3D::QTransform *torusTransforms = new Qt3D::QTransform();

    torusScale->setScale3D(QVector3D(1.5f, 1.0f, 0.5f));
    torusRotation->setAngleDeg(45.0f);
    torusRotation->setAxis(QVector3D(1, 0, 0));

    torusTransforms->appendTransform(torusScale);
    torusTransforms->appendTransform(torusRotation);

    // Torus
    Qt3D::QEntity *torusEntity = new Qt3D::QEntity();
    torusEntity->addComponent(torus);
    torusEntity->addComponent(torusTransforms);

    // Setting the FrameGraph
    rootEntity->addComponent(frameGraph);

    // Build Node Tree
    rootEntity->addChild(cameraEntity);
    rootEntity->addChild(torusEntity);

    // Set root object of the scene
    view.setRootObject(rootEntity);
    // Show window
    view.show();

    return app.exec();
}
