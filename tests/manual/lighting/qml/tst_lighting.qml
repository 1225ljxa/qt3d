/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtQuick3D module of the Qt Toolkit.
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

import QtQuick 1.0
import Qt3D 1.0
import Qt3D.Shapes 1.0

// QTBUG-17408
Item {
    id: screen

    width: aTestCase.defaultWidth * 2
    height: aTestCase.defaultHeight * 1

    Flow {
        anchors.fill: parent

        LightingTestCase {
            id: aTestCase
            text: "Positional Lighting Direct Position Test.  The light should appear to be emanating from the sphere, rotating around the middle teapot and lighting the insides of the other 4 teapots dynamically."

            onThetaChanged: {
                light.position =  Qt.vector3d(1.5 * Math.sin(theta),
                                              1.5 * Math.cos(theta), 0);
                lightSphere.position = light.position;
                }
        }

        LightingTestCase {
            text: "Directional Lighting Test.
            Lighting should look the same on all teapots"
            onThetaChanged: {
                light.direction = Qt.vector3d(1.5 * Math.sin(theta),
                                                  1.5 * Math.cos(theta),0);
            }
        }
    }
}
