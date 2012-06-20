/****************************************************************************
**
** Copyright (C) 2012 Nokia Corporation and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/
**
** This file is part of the test suite of the Qt Toolkit.
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

import QtQuick 2.0
import Qt3D 2.0
import Qt3D.Shapes 2.0

// QTBUG-18702
Item {
    id: screen
    width: parent.width; height: parent.height

    Flow {
        anchors.fill: parent

        TestCase {
            id: aTestCase
            text: "Rotation: Z axis\n(Rolling to the left)\nNumberAnimation"
            rotationAxis: Qt.vector3d(0,0,1)
            rotation: Rotation3D {
                axis: aTestCase.rotationAxis
                NumberAnimation on angle {
                    loops: Animation.Infinite
                    from: 0
                    to: 360
                    duration: 1500
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    aTestCase.visible = false
                    bTestCase.visible = true
                }
            }
        }

        TestCase {
            id: bTestCase
            text: "Rotation: Z axis\n(Rolling to the left)\nRotationAnimation"
            rotationAxis: Qt.vector3d(0,0,1)
            rotation: Rotation3D {
                axis: bTestCase.rotationAxis
                RotationAnimation on angle {
                    loops: Animation.Infinite
                    from: 0
                    to: 359
                    duration: 1500
                    direction: RotationAnimation.Clockwise
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    bTestCase.visible = false
                    cTestCase.visible = true
                }
            }
        }

        TestCase {
            id: cTestCase
            text: "Rotation: Z axis\n(Rolling to the left)\nSmoothedAnimation"
            rotationAxis: Qt.vector3d(0,0,1)
            rotation: Rotation3D {
                axis: cTestCase.rotationAxis
                SmoothedAnimation on angle {
                    loops: Animation.Infinite
                    from: 0
                    to: 360
                    duration: 1500
                    velocity: 200
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    cTestCase.visible = false
                    dTestCase.visible = true
                }
            }
        }

        TestCase {
            id: dTestCase
            text: "Rotation: Z axis\n(Rolling to the left)\nSpringAnimation"
            rotationAxis: Qt.vector3d(0,0,1)
            rotation: Rotation3D {
                axis: dTestCase.rotationAxis
                SpringAnimation on angle {
                    id: rotationAnimation
                    loops: Animation.Infinite
                    from: 0
                    to: 359
                    duration: 1500
                    spring: 2
                    damping: 0.2
                }
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    Qt.quit()
                }
            }
        }
    }
}
