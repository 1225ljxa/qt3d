import QtQuick 1.0

FocusScope {
    property alias label: textLabel.text;
    property alias value: textInput.text;
    property double delta: 0.1;
    property double max: 1;
    property double min: -1;
    property bool limitMax: false;
    property bool limitMin: false;

    width: parent.width
    height: 20

    Text {
        id: textLabel
        anchors.left: parent.left
        color: "#FFFFFF"
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: parent.width/3
        visible: parent.label !== ""
    }

    Rectangle {
        anchors.right: parent.right
        radius: 8
        border.width: 2
        border.color: "black"
        color: "white"
        width: radius*2 + textInput.width + plus.width + minus.width
        height: parent.height

        TextInput {
            id: textInput
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            horizontalAlignment: TextInput.AlignHCenter
            //FIXME: this should not be hard coded, but for some reason the display does not update properly if I use parent.width
            width: 130
            focus: true
            validator: DoubleValidator{}
            onAccepted: {
                parent.parent.update(text)

            }
        }
        Image {
            id: plus
            source: "plus.png"
            anchors.left: textInput.right
            anchors.verticalCenter: parent.verticalCenter
            MouseArea { id: plusMouseArea; anchors.fill: parent; onPressed: incDelta() }
            Timer {
                interval: 100;
                running: plusMouseArea.pressedButtons & Qt.LeftButton
                repeat: true
                onTriggered: incDelta()
            }
        }
        Image {
            id: minus
            source: "minus.png"
            anchors.right: textInput.left
            anchors.verticalCenter: parent.verticalCenter
            MouseArea { id: minusMouseArea; anchors.fill: parent; onPressed: decDelta() }
            Timer {
                interval: 100;
                running: minusMouseArea.pressedButtons & Qt.LeftButton
                repeat: true
                onTriggered: decDelta()
            }
        }
    }

    function updateMe() {
        update(textInput.text)
    }

    function incDelta() {
        var t = value*1 + delta;
        if (limitMax)
            update(t>max ? max : t);
        else
            update(t);
        console.log("updated! " + t);
    }

    function decDelta() {
        var t = value*1 - delta;
        if (limitMin)
            update(t<min ? min : t);
        else
            update(t);
        console.log("updated! " + t);
    }

    Keys.onUpPressed: incDelta()
    Keys.onDownPressed: decDelta()
    Keys.onReturnPressed: updateMe()
}

