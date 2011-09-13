import QtQuick 2.0
import Qt3D 1.0
import Qt3D.Shapes 1.0

Image {
    source: "sky.png"

    Viewport  {
        width: 640
        height: 480

        camera: Camera {
            eye: Qt.vector3d(2,5,20);
        }

        picking: true

        Cube {
            onClicked: console.log("Cube clicked")
            effect: Effect {
                color: "#aaca00"
            }
        }
    }

    Text {
        id: instructionsText
        text: "Cube + back image. Drag to rotate, click & check log."
    }
}

