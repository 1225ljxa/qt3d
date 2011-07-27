TEMPLATE = app
TARGET = solarsystem
CONFIG += qt warn_on
!package: CONFIG += qt3d

SOURCES = solarsystem.cpp \
    main.cpp
HEADERS = solarsystem.h
RESOURCES += solarsystem.qrc
DEPENDPATH += resources

CONFIG += qt3d_deploy_pkg
include(../../../pkg.pri)

OTHER_FILES += \
    resources/moon-texture.jpg \
    resources/planet.jpg \
    resources/solar.jpg \
    resources/solar.fsh \
    resources/solar.vsh

OTHER_FILES += \
    solarsystem.rc

RC_FILE = solarsystem.rc

symbian {
    ICON = ../qt3d.svg
}
