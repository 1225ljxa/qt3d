TEMPLATE = app
TARGET = tst_model3ds
CONFIG += qt warn_on

QT += qt3dquick

SOURCES += main.cpp

INSTALL_DIRS = qml

CONFIG += qt3d_deploy_qml qt3dquick_deploy_pkg
include(../../../pkg.pri)
qtcAddDeployment()
