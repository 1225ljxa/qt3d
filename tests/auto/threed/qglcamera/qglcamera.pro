TARGET = tst_qglcamera
CONFIG += testcase
TEMPLATE=app
QT += testlib 3d
CONFIG += warn_on

INCLUDEPATH += ../../../../threed/viewing
VPATH += ../../../../threed/viewing

SOURCES += \
    tst_qglcamera.cpp
