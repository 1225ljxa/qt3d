TEMPLATE = app
TARGET = pageflip
CONFIG += qt warn_on
QT += qt3d

include(../../../pkg.pri)

SOURCES = pageflip.cpp pageflipmath.cpp
HEADERS = pageflipmath_p.h
RESOURCES = pageflip.qrc

OTHER_FILES += \
    pageflip.rc

RC_FILE = pageflip.rc
