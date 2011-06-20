TEMPLATE = app
TARGET = basket_qml
CONFIG += qt warn_on

SOURCES += main.cpp
HEADERS += ../qmlres.h

QT += declarative

!package:DESTDIR = ../../../bin
# for cleanup on Windows platforms - avoid deletion prompts
win32 {
    QMAKE_DEL_FILE = del /q
    QMAKE_DEL_DIR = rmdir /s /q
}

package {
    maemo {
        applnk.files = basket_qml.desktop
        applnk.path = /usr/share/applications

        icons.files = icon-l-qtquick3d.png
        icons.path = /usr/share/themes/base/meegotouch/icons
        INSTALLS += icons applnk
    }

    target.path = $$[QT_INSTALL_BINS]
    INSTALLS += target
}

OTHER_FILES += \
    basket_qml.rc

RC_FILE = basket_qml.rc

RESOURCES += \
    basket.qrc

