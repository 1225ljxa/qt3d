HEADERS += \
    $$PWD/qinputaspect.h \
    $$PWD/qkeyboardcontroller.h \
    $$PWD/qkeyboardcontroller_p.h \
    $$PWD/qkeyboardinput.h \
    $$PWD/qkeyboardinput_p.h \
    $$PWD/qinputaspect_p.h \
    $$PWD/qkeyevent.h \
    $$PWD/qmousecontroller.h \
    $$PWD/qmousehandler.h \
    $$PWD/qmousehandler_p.h \
    $$PWD/qmousecontroller_p.h \
    $$PWD/qmouseevent.h \
    $$PWD/qinputdeviceplugin.h \
    $$PWD/qactioninput.h \
    $$PWD/qaction.h \
    $$PWD/qaxisinput.h \
    $$PWD/qaxis.h \
    $$PWD/qlogicaldevice.h \
    $$PWD/qinputdeviceintegration.h \
    $$PWD/qinputdeviceintegration_p.h \
    $$PWD/qabstractphysicaldevice.h \
    $$PWD/qinputdeviceintegrationfactory_p.h \
    $$PWD/qaxissetting.h \
    $$PWD/qabstractphysicaldevice_p.h \
    $$PWD/qgenericinputdevice.h \
    $$PWD/qabstractactioninput.h \
    $$PWD/qabstractaggregateactioninput.h \
    $$PWD/qinputchord.h \
    $$PWD/qinputsequence.h \
    $$PWD/qabstractaggregateactioninput_p.h \
    $$PWD/qinputsettings.h \
    $$PWD/qinputsettings_p.h

SOURCES += \
    $$PWD/qinputaspect.cpp \
    $$PWD/qkeyboardcontroller.cpp \
    $$PWD/qkeyboardinput.cpp \
    $$PWD/qkeyevent.cpp \
    $$PWD/qmousehandler.cpp \
    $$PWD/qmousecontroller.cpp \
    $$PWD/qmouseevent.cpp \
    $$PWD/qinputdeviceplugin.cpp \
    $$PWD/qactioninput.cpp \
    $$PWD/qaction.cpp \
    $$PWD/qaxisinput.cpp \
    $$PWD/qaxis.cpp \
    $$PWD/qlogicaldevice.cpp \
    $$PWD/qinputdeviceintegration.cpp \
    $$PWD/qabstractphysicaldevice.cpp \
    $$PWD/qinputdeviceintegrationfactory.cpp \
    $$PWD/qaxissetting.cpp \
    $$PWD/qgenericinputdevice.cpp \
    $$PWD/qabstractactioninput.cpp \
    $$PWD/qabstractaggregateactioninput.cpp \
    $$PWD/qinputchord.cpp \
    $$PWD/qinputsequence.cpp \
    $$PWD/qinputsettings.cpp

qtHaveModule(gamepad) {
    QT += gamepad
    DEFINES += HAVE_QGAMEPAD
    HEADERS += $$PWD/qgamepadinput.h
    SOURCES += $$PWD/qgamepadinput.cpp
}

INCLUDEPATH += $$PWD

