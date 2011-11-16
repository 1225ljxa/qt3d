import QtQuick 1.0
import Qt3D 1.0
import ModelTweak 1.0

Row {
    x: 16
    y: parent.height/2 + 16
    width: parent.width/2 - 16 - 16
    height: parent.height/2 - 16 - 16
    spacing: 16

    Column {
        width: parent.width/2
        height: parent.height
        spacing: 4

        // POSITION
        Text {
            text: "Position";
            color: "#FFFFFF"
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
        }
        ValueField {
            focus: true
            id: posX
            label: "X:"
            value: transformTranslate.translate.x
            function update (f)  {
                transformTranslate.translate = Qt.vector3d(f, transformTranslate.translate.y, transformTranslate.translate.z);
            }
            Keys.onTabPressed:   { updateMe(); posY.focus = true; }
        }
        ValueField {
            id: posY
            label: "Y:"
            value: transformTranslate.translate.y
            function update (f)  {
                transformTranslate.translate = Qt.vector3d(transformTranslate.translate.x, f, transformTranslate.translate.z);
            }
            Keys.onTabPressed:   { updateMe(); posZ.focus = true; }
        }
        ValueField {
            id: posZ
            label: "Z:"
            value: transformTranslate.translate.z
            function update (f)  {
                transformTranslate.translate = Qt.vector3d(transformTranslate.translate.x, transformTranslate.translate.y, f);
            }
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
            delta: 1
            min: 0;   limitMin: true
            max: 360; limitMax: true
            value: transformRotateX.angle
            function update (f)  { transformRotateX.angle = f }
            Keys.onTabPressed:   { updateMe(); rotY.focus = true; }
        }
        ValueField {
            id: rotY
            label: "Y:"
            delta: 1
            min: 0;   limitMin: true
            max: 360; limitMax: true
            value: transformRotateY.angle
            function update (f)  { transformRotateY.angle = f }
            Keys.onTabPressed:   { updateMe(); rotZ.focus = true; }
        }
        ValueField {
            id: rotZ
            label: "Z:"
            delta: 1
            min: 0;   limitMin: true
            max: 360; limitMax: true
            value: transformRotateZ.angle
            function update (f)  { transformRotateZ.angle = f }
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
            min: 0; limitMin: true
            value: transformScale.scale.x
            function update (f)  { transformScale.scale = Qt.vector3d(f, transformScale.scale.y, transformScale.scale.z); }
            Keys.onTabPressed:   { updateMe(); scaleY.focus = true; }
        }
        ValueField {
            id: scaleY
            label: "Y:"
            min: 0; limitMin: true
            value: transformScale.scale.y
            function update (f)  { transformScale.scale = Qt.vector3d(transformScale.scale.x, f, transformScale.scale.z); }
            Keys.onTabPressed:   { updateMe(); scaleZ.focus = true; }
        }
        ValueField {
            id: scaleZ
            label: "Z:"
            min: 0; limitMin: true
            value: transformScale.scale.z
            function update (f)  { transformScale.scale = Qt.vector3d(transformScale.scale.x, transformScale.scale.y, f); }
            Keys.onTabPressed:   { updateMe(); posX.focus = true; }
        }

    }
    Column {
        width: parent.width/2 - 16
        height: parent.height
        spacing: 8

        Row {
            width: parent.width
            height: childrenRect.height
            spacing: 8

            GrayButton {
                width: parent.width / 2 - 4
                buttonText: "Save!"
                function onClicked() {
                    var saveData =
                            "import QtQuick 1.0\n" +
                            "import Qt3D 1.0\n" +
                            "\n" +
                            "Item3D {\n" +
                            "    Translation3D {\n" +
                            "        id: transformTranslate\n" +
                            "        translate: Qt.vector3d(" +
                                transformTranslate.translate.x + ", " +
                                transformTranslate.translate.y + ", " +
                                transformTranslate.translate.z + ")\n" +
                            "    }\n" +
                            "\n" +
                            "    Rotation3D {\n" +
                            "        id: transformRotateX\n" +
                            "        angle: " + transformRotateX.angle + "\n" +
                            "        axis: Qt.vector3d(1, 0, 0)\n" +
                            "    }\n" +
                            "\n" +
                            "    Rotation3D {\n" +
                            "        id: transformRotateY\n" +
                            "        angle: " + transformRotateY.angle + "\n" +
                            "        axis: Qt.vector3d(0, 1, 0)\n" +
                            "    }\n" +
                            "\n" +
                            "    Rotation3D {\n" +
                            "        id: transformRotateZ\n" +
                            "        angle: " + transformRotateZ.angle + "\n" +
                            "        axis: Qt.vector3d(0, 0, 1)\n" +
                            "    }\n" +
                            "\n" +
                            "    Scale3D {\n" +
                            "        id: transformScale\n" +
                            "        scale: Qt.vector3d(" +
                                transformScale.scale.x + ", " +
                                transformScale.scale.y + ", " +
                                transformScale.scale.z + ")\n" +
                            "    }\n" +
                            "\n" +
                            "    Mesh {\n" +
                            "        id: source_mesh\n" +
                            "        source: \"" + targetMesh + "\"\n" +
                            "    }\n" +
                            "\n" +
                            "    mesh: source_mesh\n" +
                            "    effect: Effect {}\n" +
                            "    transform: [\n" +
                            "        transformScale\n" +
                            "        transformRotateX,\n" +
                            "        transformRotateY,\n" +
                            "        transformRotateZ,\n" +
                            "        transformTranslate,\n" +
                            "    ]\n" +
                            "}\n";

                    quickSave.filename = source_mesh.source
                    quickSave.data = saveData
                    var result = quickSave.save
                    console.log("If there was an error it will be after here:" + result)
                }
            }

            GrayButton {
                width: parent.width / 2 - 4
                buttonText: "Load!"
                function onClicked() { console.log(quickLoad.openModelFile); }
            }

        }

        Row {
            width: parent.width
            height: childrenRect.height
            spacing: 8

            GrayButton {
                width: parent.width / 2 - 4
                buttonText: "Help!"
                function onClicked() { helpOverlay.visible = true }
            }

            GrayButton {
                width: parent.width / 2 - 4
                buttonText: "To let."
                function onClicked() { console.log("Dunno what this does yet...") }
            }
        }
    }
}
