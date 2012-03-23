TEMPLATE = app
TARGET = tst_scaling
CONFIG += qt warn_on

QT += 3dquick

SOURCES += main.cpp

ICON_FILE = ../icon.png

QML_FILES = \
    qml/desktop.qml \
    qml/TestScaling.qml

QML_INFRA_FILES = \
    $$QML_FILES

CATEGORY = manual
include(../../../pkg.pri)

OTHER_FILES += \
    mt.qml \
    $$QML_INFRA_FILES
