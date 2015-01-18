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

import Qt3D 2.0
import Qt3D.Render 2.0

Effect {
    techniques : [
        // OpenGL 3.1
        Technique {
            openGLFilter {api : OpenGLFilter.Desktop; profile : OpenGLFilter.Core; minorVersion : 1; majorVersion : 3 }
            renderPasses : RenderPass {
                annotations : Annotation { name : "pass"; value : "final" }
                shaderProgram : ShaderProgram {
                    id : finalShaderGL3
                    vertexShaderCode:
                        "#version 140

                        in vec4 vertexPosition;
                        uniform mat4 modelMatrix;

                        void main()
                        {
                            gl_Position = modelMatrix * vertexPosition;
                        }
                        "
                    fragmentShaderCode:
                        "#version 140

                        uniform sampler2D color;
                        uniform sampler2D position;
                        uniform sampler2D normal;
                        uniform vec2 winSize;

                        out vec4 fragColor;

                        struct PointLight
                        {
                            vec3 position;
                            vec3 direction;
                            vec4 color;
                            float intensity;
                        };

                        const int lightCount = 3;
                        uniform PointLightBlock {
                             PointLight lights[lightCount];
                        } pointLights;

                        void main()
                        {
                            vec2 texCoord = gl_FragCoord.xy / winSize;
                            vec4 col = texture2D(color, texCoord);
                            vec3 pos = texture2D(position, texCoord).xyz;
                            vec3 norm = texture2D(normal, texCoord).xyz;

                            vec4 lightColor;
                            for (int i = 0; i < 3; i++) {
                                vec3 s = normalize(pointLights.lights[i].position - pos);
                                lightColor += pointLights.lights[i].color * (pointLights.lights[i].intensity * max(dot(s, norm), 0.0));
                            }
                            lightColor /= float(lightCount);
                            fragColor = col * lightColor;
                        }
                                "
                }
            }
        },
        // OpenGL 2.0 with FBO extension
        Technique {
            openGLFilter {api : OpenGLFilter.Desktop; profile : OpenGLFilter.None; minorVersion : 0; majorVersion : 2 }
            renderPasses : RenderPass {
                annotations : Annotation { name : "pass"; value : "final" }
                shaderProgram : ShaderProgram {
                    id : finalShaderGL2
                    vertexShaderCode:
                        "#version 110

                        attribute vec4 vertexPosition;
                        uniform mat4 modelMatrix;

                        void main()
                        {
                            gl_Position = modelMatrix * vertexPosition;
                        }
                                "
                    fragmentShaderCode:
                        "#version 110

                        uniform sampler2D color;
                        uniform sampler2D position;
                        uniform sampler2D normal;
                        uniform vec2 winSize;

                        struct PointLight
                        {
                            vec3 position;
                            vec3 direction;
                            vec4 color;
                            float intensity;
                        };

                        const int lightCount = 3;
                        uniform PointLight pointLights[lightCount];

                        void main()
                        {
                            vec2 texCoord = gl_FragCoord.xy / winSize;
                            vec4 col = texture2D(color, texCoord);
                            vec3 pos = texture2D(position, texCoord).xyz;
                            vec3 norm = texture2D(normal, texCoord).xyz;

                            vec4 lightColor;
                            for (int i = 0; i < 3; i++) {
                                vec3 s = normalize(pointLights[i].position - pos);
                                lightColor += pointLights[i].color * (pointLights[i].intensity * max(dot(s, norm), 0.0));
                            }
                            lightColor /= float(lightCount);
                            gl_FragColor = col * lightColor;
                        }
                    "
                }
            }
        }]
}
