TEMPLATE = app
TARGET = penguin
CONFIG += qt warn_on
QT += qt3d
SOURCES = modelview.cpp \
    main.cpp
HEADERS = modelview.h
RESOURCES = model.qrc
DESTDIR = $$QT.qt3d.bins/qt3d/tutorials
