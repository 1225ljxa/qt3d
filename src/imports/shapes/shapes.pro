TEMPLATE = lib
TARGET  = qshapesqmlplugin
CONFIG += qt plugin

QT += declarative quick qt3d qt3dquick

DESTDIR = $$QT.qt3dquick.imports/Qt3D/Shapes
target.path = $$[QT_INSTALL_IMPORTS]/Qt3D/Shapes
INSTALLS += target

SOURCES += \
    shapes.cpp \
    line.cpp \
    point.cpp \
    spheremesh.cpp \
    capsulemesh.cpp \
    cylindermesh.cpp
HEADERS += \
    line.h \
    point.h \
    spheremesh.h \
    spheremesh_p.h \
    capsulemesh.h \
    cylindermesh.h \
    capsulemesh_p.h \
    cylindermesh_p.h

QML_FILES = \
    Cube.qml \
    Sphere.qml \
    Quad.qml \
    Teapot.qml \
    Capsule.qml \
    Cylinder.qml

QML_INFRA_FILES += \
    $$QML_FILES \
    qmldir \
    library.xml \
    plugins.qmltypes \
    cube.obj \
    quad.obj \
    teapot.bez
# see the file README.library_xml for more on library.xml

package {
    copyqmlinfra_install.files = $$QML_INFRA_FILES
    copyqmlinfra_install.path = $$[QT_INSTALL_IMPORTS]/Qt3D/Shapes
    INSTALLS += copyqmlinfra_install
} else {
    copyqmlinfra.input = QML_INFRA_FILES
    copyqmlinfra.output = $$target.path/${QMAKE_FILE_IN_BASE}${QMAKE_FILE_EXT}
    copyqmlinfra.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
    copyqmlinfra.CONFIG += no_link_no_clean
    copyqmlinfra.variable_out = PRE_TARGETDEPS
    QMAKE_EXTRA_COMPILERS += copyqmlinfra
}

OTHER_FILES += \
    README.plugins_types \
    README.library_xml

OTHER_FILES += $$QML_FILES
















