TEMPLATE = app
TARGET = cylinder
CONFIG += qt warn_on
QT += qt3d

SOURCES = cylinderview.cpp main.cpp
HEADERS = cylinderview.h
RESOURCES = cylinder.qrc

CONFIG += qt3d_deploy_pkg
include(../../../pkg.pri)

OTHER_FILES += \
    cylinder.rc

RC_FILE = cylinder.rc
