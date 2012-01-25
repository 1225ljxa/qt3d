TEMPLATE = app
TARGET = lander
CONFIG += qt warn_on
QT += declarative quick

SOURCES += main.cpp

ICON_FILE = ../icon.png

QML_FILES = \
    qml/desktop.qml \
    qml/Jet.qml \
    qml/Lander.qml

QML_INFRA_FILES = \
    $$QML_FILES \
    qml/flame.xcf \
    qml/flame2.xcf \
    qml/nebula.jpg \
    qml/dropshadow.png \
    qml/flame.png \
    qml/flame2.png \
    qml/rusty.png

QML_MESHES_FILES = \
    qml/lunar-lander.3ds \
    qml/meshes/lunar-lander.3ds \
    qml/meshes/lunar-lander.obj \
    qml/meshes/lunar-landscape.3ds \
    qml/meshes/lunar-landscape.obj

CATEGORY = examples
include(../../../pkg.pri)

INSTALL_DIRS = qml
mt: INSTALL_FILES = mt.qml

OTHER_FILES += \
    lander.rc \
    $$QML_INFRA_FILES

RC_FILE = lander.rc
