TEMPLATE = app
TARGET = teaservice
CONFIG += qt warn_on

QT += qt3d

SOURCES = teaservice.cpp meshobject.cpp sceneobject.cpp perpixeleffect.cpp
HEADERS = meshobject.h sceneobject.h perpixeleffect.h
RESOURCES = teaservice.qrc

OTHER_FILES += \
    teaservice.rc

RC_FILE = teaservice.rc
