TEMPLATE = app
TARGET = basket
CONFIG += qt warn_on
!package: CONFIG += qt3d

include(../../../qt3d_pkg_dep.pri)

SOURCES = basketview.cpp main.cpp
HEADERS = basketview.h basket_data.h
RESOURCES = basket.qrc

OTHER_FILES += \
    basket.rc

RC_FILE = basket.rc
