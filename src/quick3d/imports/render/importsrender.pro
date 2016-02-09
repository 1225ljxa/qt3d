CXX_MODULE = qml
TARGET = quick3drenderplugin
TARGETPATH = Qt3D/Render

QT += core-private qml qml-private 3dcore 3drender 3dquick 3dquick-private 3dquickrender-private

HEADERS += \
    qt3dquick3drenderplugin.h

SOURCES += \
    qt3dquick3drenderplugin.cpp

include(./defaults/defaults.pri)

OTHER_FILES += qmldir

load(qml_plugin)

