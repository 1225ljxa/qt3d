TEMPLATE = app
TARGET = teapot_qml
CONFIG += qt warn_on
CONFIG += qt3dquick


SOURCES += main.cpp
HEADERS += ../qmlres.h

DESTDIR = ../../../bin

INSTALL_DIRS = qml

CONFIG += qt3d_deploy_qml
include(../../../pkg.pri)
qtcAddDeployment()
