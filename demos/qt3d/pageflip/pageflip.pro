TEMPLATE = app
TARGET = pageflip
CONFIG += qt warn_on
QT += widgets qt3d

SOURCES = pageflip.cpp pageflipmath.cpp
HEADERS = pageflipmath_p.h
RESOURCES = pageflip.qrc

CONFIG += qt3d_deploy_pkg
include(../../../pkg.pri)

OTHER_FILES += \
    pageflip.rc

RC_FILE = pageflip.rc
