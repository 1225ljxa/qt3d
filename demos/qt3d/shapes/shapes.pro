TEMPLATE = app
TARGET = shapes
CONFIG += qt warn_on
QT += widgets qt3d

SOURCES = shapes.cpp

CONFIG += qt3d_deploy_pkg
include(../../../pkg.pri)

OTHER_FILES += \
    shapes.rc

RC_FILE = shapes.rc
