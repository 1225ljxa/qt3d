TEMPLATE = app
TARGET = cube_qml
QT += declarative quick
CONFIG += qt warn_on

SOURCES += main.cpp

INSTALL_DIRS = qml
mt: INSTALL_FILES = mt.qml

CONFIG += qt3d_deploy_qml
include(../../../pkg.pri)
qtcAddDeployment()

OTHER_FILES += \
    cube_qml.rc

RC_FILE = cube_qml.rc
