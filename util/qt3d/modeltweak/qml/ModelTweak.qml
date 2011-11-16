/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtQuick3D examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 1.0
import Qt3D 1.0

Rectangle {
    id: mainwindow
    width: 1024
    height: 768
    color: "#444444"
    border.color: "black"
    property string targetMesh: "meshes/monkey.3ds";

    Translation3D {
        id: transformTranslate
        translate: Qt.vector3d(0, 0, 0)
    }

    Rotation3D {
        id: transformRotate
        angle: 0
        axis: Qt.vector3d(0, 0, 1)
    }

    Scale3D {
        id: transformScale
        scale: Qt.vector3d(1, 1, 1)
    }

    Mesh {
        id: source_mesh
        source: mainwindow.targetMesh
    }

    ModelViewport {
        id: viewport2
        x: 0
        y: 0
        camera.eye: Qt.vector3d(10, 0, 0)
    }

    ModelViewport {
        id: viewport
        x: parent.width/2
        y: 0
        camera.eye: Qt.vector3d(0, 0, 10)
    }

    ModelViewport {
        id: viewport3
        x: parent.width/2
        y: parent.height/2
        camera.eye: Qt.vector3d(0, 10, 0)
        camera.upVector: Qt.vector3d(0, 0, -1)
    }

    Rectangle {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 10
        anchors.bottom: parent.bottom
        radius: 10
        border.width: 1
        border.color: "black"
        color: "white"
        width: parent.width - 20
        height: 20

        Text {
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            text: source_mesh.source
        }
    }

    Column {
        anchors.left: parent.left
        anchors.leftMargin: 32
        anchors.topMargin: 32
        y: parent.height/2 + 32
        width: 200
        height: 200

        // POSITION
        Text {
            text: "Position";
            color: "#FFFFFF"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        ValueField {
            focus: true
            id: posX
            label: "X:"
            value: transformTranslate.translate.x
            function update (f)  { transformTranslate.translate = Qt.vector3d(f, transformTranslate.translate.y, transformTranslate.translate.z); }
            Keys.onTabPressed:   { updateMe(); posY.focus = true; }
        }
        ValueField {
            id: posY
            label: "Y:"
            value: transformTranslate.translate.y
            function update (f)  { transformTranslate.translate = Qt.vector3d(transformTranslate.translate.x, f, transformTranslate.translate.z); }
            Keys.onTabPressed:   { updateMe(); posZ.focus = true; }
        }
        ValueField {
            id: posZ
            label: "Z:"
            value: transformTranslate.translate.z
            function update (f)  { transformTranslate.translate = Qt.vector3d(transformTranslate.translate.x, transformTranslate.translate.y, f); }
            Keys.onTabPressed:   { updateMe(); rotX.focus = true; }
        }

        // ROTATE
        Text {
            text: "Rotation";
            color: "#FFFFFF"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        ValueField {
            id: rotX
            label: "X:"
            value: transformRotate.axis.x
            function update (f)  { transformRotate.axis.x = f }
            Keys.onTabPressed:   { updateMe(); rotY.focus = true; }
        }
        ValueField {
            id: rotY
            label: "Y:"
            value: transformRotate.axis.y
            function update (f)  { transformRotate.axis.y = f; }
            Keys.onTabPressed:   { updateMe(); rotZ.focus = true; }
        }
        ValueField {
            id: rotZ
            label: "Z:"
            value: transformRotate.axis.z
            function update (f)  { transformRotate.axis.z = f }
            Keys.onTabPressed:   { updateMe(); angle.focus = true; }
        }
        ValueField {
            id: angle
            label: "Angle:"
            value: transformRotate.angle
            delta: 1
            max: 360
            min: 0
            limit: true
            function update (f)  { transformRotate.angle = f }
            Keys.onTabPressed:   { updateMe(); scaleX.focus = true; }
        }

        // SCALE
        Text {
            text: "Scale";
            color: "#FFFFFF"
            anchors.horizontalCenter: parent.horizontalCenter
        }
        ValueField {
            id: scaleX
            label: "X:"
            value: transformScale.scale.x
            function update (f)  { transformScale.scale = Qt.vector3d(f, transformScale.scale.y, transformScale.scale.z); }
            Keys.onTabPressed:   { updateMe(); scaleY.focus = true; }
        }
        ValueField {
            id: scaleY
            label: "Y:"
            value: transformScale.scale.y
            function update (f)  { transformScale.scale = Qt.vector3d(transformScale.scale.x, f, transformScale.scale.z); }
            Keys.onTabPressed:   { updateMe(); scaleZ.focus = true; }
        }
        ValueField {
            id: scaleZ
            label: "Z:"
            value: transformScale.scale.z
            function update (f)  { transformScale.scale = Qt.vector3d(transformScale.scale.x, transformScale.scale.y, f); }
            Keys.onTabPressed:   { updateMe(); posX.focus = true; }
        }

        // CAMERA
        Text {
            text: "Camera";
            color: "#FFFFFF"
            anchors.horizontalCenter: parent.horizontalCenter
        }
    }
}
