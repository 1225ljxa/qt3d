TEMPLATE = app
TARGET = model_viewer
QT += declarative quick widgets
CONFIG += qt warn_on

SOURCES += main.cpp

INSTALL_DIRS = qml
mt: INSTALL_FILES = mt.qml

CONFIG += qt3d_deploy_qml
include(../../../pkg.pri)
qtcAddDeployment()

OTHER_FILES += \
    model_viewer.rc

RC_FILE = model_viewer.rc
