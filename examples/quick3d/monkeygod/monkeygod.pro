TEMPLATE = app
TARGET = monkeygod
CONFIG += qt warn_on
!package: CONFIG += qt3dquick

SOURCES += main.cpp

include(../../../qt3dquick_pkg_dep.pri)
include(../../../qml_pkg.pri)

OTHER_FILES += \
    monkeygod.rc

RC_FILE = monkeygod.rc
