TEMPLATE = app
TARGET = forest_qml
CONFIG += qt warn_on
!package: CONFIG += qt3dquick

SOURCES += main.cpp

include(../../../qt3dquick_pkg_dep.pri)
include(../../../qml_pkg.pri)

OTHER_FILES += \
    forest.rc

RC_FILE = forest.rc
