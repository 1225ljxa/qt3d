TEMPLATE = app
TARGET = basket_qml
CONFIG += qt warn_on
!package: CONFIG += qt3dquick

SOURCES += main.cpp

CONFIG += qt3d_deploy_qml qt3dquick_deploy_pkg
include(../../../pkg.pri)

OTHER_FILES += \
    basket_qml.rc

RC_FILE = basket_qml.rc

RESOURCES += \
    basket.qrc

