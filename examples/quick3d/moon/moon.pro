TEMPLATE = app
TARGET = moon
CONFIG += qt warn_on
QT += declarative quick

SOURCES += main.cpp

INSTALL_DIRS = qml
mt: INSTALL_FILES = mt.qml

CONFIG += qt3d_deploy_qml
include(../../../pkg.pri)
qtcAddDeployment()

OTHER_FILES += \
    moon.rc

RC_FILE = moon.rc
