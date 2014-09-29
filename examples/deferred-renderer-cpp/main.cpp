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

#include <Qt3DCore/QEntity>
#include <Qt3DCore/Window>
#include <Qt3DRenderer/RendererAspect>
#include <Qt3DRenderer/QFrameGraph>
#include <Qt3DRenderer/QMaterial>
#include <Qt3DRenderer/QSphereMesh>
#include <Qt3DRenderer/QPlaneMesh>
#include <Qt3DRenderer/QLayer>
#include <Qt3DRenderer/QParameter>
#include <Qt3DCore/QCamera>
#include <Qt3DCore/QCameraLens>
#include <Qt3DCore/QTransform>
#include <Qt3DCore/QLookAtTransform>
#include <Qt3DCore/QRotateTransform>
#include <Qt3DCore/QTranslateTransform>
#include <Qt3DRenderer/QPointLight>

#include <QGuiApplication>

#include "gbuffer.h"
#include "deferredrenderer.h"
#include "finaleffect.h"
#include "sceneeffect.h"

int main(int ac, char **av)
{
    QGuiApplication app(ac, av);

    Qt3D::Window view;
    view.registerAspect(new Qt3D::RendererAspect());

    // Root entity
    Qt3D::QEntity *rootEntity = new Qt3D::QEntity();
    rootEntity->setObjectName(QStringLiteral("rootEntity"));

    // Shared Components
    GBuffer *gBuffer = new GBuffer();
    FinalEffect *finalEffect = new FinalEffect();
    SceneEffect *sceneEffect = new SceneEffect();

    Qt3D::QSphereMesh *sphereMesh = new Qt3D::QSphereMesh();
    sphereMesh->setRings(50);
    sphereMesh->setSlices(100);

    Qt3D::QLayer *sceneLayer = new Qt3D::QLayer();
    sceneLayer->setName("scene");

    Qt3D::QLayer *quadLayer = new Qt3D::QLayer();
    quadLayer->setName("screenQuad");

    Qt3D::QPlaneMesh *planeMesh = new Qt3D::QPlaneMesh();
    planeMesh->setMeshResolution(QSize(2, 2));
    planeMesh->setWidth(2.0f);
    planeMesh->setHeight(2.0f);

    Qt3D::QPointLight *light1 = new Qt3D::QPointLight();
    light1->setColor(Qt::white);
    light1->setIntensity(1.5f);

    rootEntity->addComponent(light1);

    // Scene Camera
    Qt3D::QCamera *camera = new Qt3D::QCamera();
    Qt3D::QCameraLens *lens = new Qt3D::QCameraLens();

    lens->setAspectRatio(16.0f/9.0f);
    lens->setFieldOfView(45.0f);
    lens->setNearPlane(0.01f);
    lens->setFarPlane(1000.0f);
    lens->setProjectionType(Qt3D::QCameraLens::PerspectiveProjection);

    Qt3D::QTransform *transform = new Qt3D::QTransform();
    Qt3D::QLookAtTransform *lookAt = new Qt3D::QLookAtTransform();
    lookAt->setPosition(QVector3D(10.0f, 10.0f, -25.0f));
    lookAt->setUpVector(QVector3D(0.0f, 1.0f, 0.0f));
    lookAt->setViewCenter(QVector3D(0.0f, 0.0f, 10.0f));
    transform->appendTransform(lookAt);

    camera->setLens(lens);
    camera->setTransform(transform);
    view.setCamera(camera);

    // FrameGraph
    Qt3D::QFrameGraph *frameGraph = new Qt3D::QFrameGraph();
    DeferredRenderer *deferredRenderer = new DeferredRenderer();
    deferredRenderer->setClearColor(Qt::black);
    deferredRenderer->setRect(QRectF(0.0f, 0.0f, 1.0f, 1.0f));
    deferredRenderer->setFinalPassCriteria(finalEffect->passCriteria());
    deferredRenderer->setGeometryPassCriteria(sceneEffect->passCriteria());
    deferredRenderer->setGBuffer(gBuffer);
    deferredRenderer->setSceneCamera(camera);
    deferredRenderer->setGBufferLayer(sceneLayer->name());
    deferredRenderer->setScreenQuadLayer(quadLayer->name());

    frameGraph->setActiveFrameGraph(deferredRenderer);
    rootEntity->addComponent(frameGraph);

    // Scene Content
    Qt3D::QEntity *sphereOne = new Qt3D::QEntity();
    Qt3D::QMaterial *sphereOneMaterial = new Qt3D::QMaterial();

    sphereOneMaterial->setEffect(sceneEffect);
    sphereOneMaterial->addParameter(new Qt3D::QParameter(QStringLiteral("meshColor"), QColor(Qt::blue)));

    Qt3D::QTranslateTransform *sphereOneTranslate = new Qt3D::QTranslateTransform();
    sphereOneTranslate->setDx(-10.0f);
    sphereOneTranslate->setDy(0.0f);
    sphereOneTranslate->setDz(25.0f);

    Qt3D::QPointLight *light2 = new Qt3D::QPointLight();
    light2->setColor(Qt::white);
    light2->setIntensity(1.5f);

    sphereOne->addComponent(new Qt3D::QTransform(sphereOneTranslate));
    sphereOne->addComponent(sphereOneMaterial);
    sphereOne->addComponent(sphereMesh);
    sphereOne->addComponent(sceneLayer);
    sphereOne->addComponent(light2);

    Qt3D::QEntity *sphereTwo = new Qt3D::QEntity();
    Qt3D::QMaterial *sphereTwoMaterial = new Qt3D::QMaterial();
    sphereTwoMaterial->setEffect(sceneEffect);
    sphereTwoMaterial->addParameter(new Qt3D::QParameter(QStringLiteral("meshColor"), QColor(Qt::yellow)));

    Qt3D::QPointLight *light3 = new Qt3D::QPointLight();
    light3->setColor(Qt::blue);
    light3->setIntensity(0.5f);

    sphereTwo->addComponent(sphereMesh);
    sphereTwo->addComponent(sceneLayer);
    sphereTwo->addComponent(sphereTwoMaterial);
    sphereTwo->addComponent(light3);

    // Screen Quad
    Qt3D::QEntity *screenQuad = new Qt3D::QEntity();
    Qt3D::QMaterial *screenQuadMaterial = new Qt3D::QMaterial();
    screenQuadMaterial->addParameter(new Qt3D::QParameter(QStringLiteral("position"), gBuffer->positionTexture()));
    screenQuadMaterial->addParameter(new Qt3D::QParameter(QStringLiteral("normal"), gBuffer->normalTexture()));
    screenQuadMaterial->addParameter(new Qt3D::QParameter(QStringLiteral("color"), gBuffer->colorTexture()));
    screenQuadMaterial->addParameter(new Qt3D::QParameter(QStringLiteral("winSize"), QSize(1024, 1024)));
    screenQuadMaterial->setEffect(finalEffect);

    Qt3D::QRotateTransform *screenPlaneRotation = new Qt3D::QRotateTransform();
    screenPlaneRotation->setAngleDeg(90);
    screenPlaneRotation->setAxis(QVector3D(1.0f, 0.0f, 0.0f));

    screenQuad->addComponent(quadLayer);
    screenQuad->addComponent(new Qt3D::QTransform(screenPlaneRotation));
    screenQuad->addComponent(screenQuadMaterial);
    screenQuad->addComponent(planeMesh);

    rootEntity->addChild(sphereOne);
    rootEntity->addChild(sphereTwo);
    rootEntity->addChild(screenQuad);

    // Set root object of the scene
    view.setRootObject(rootEntity);
    // Show window
    view.show();

    return app.exec();
}
