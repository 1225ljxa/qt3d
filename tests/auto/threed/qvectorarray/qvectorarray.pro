TARGET = tst_qvectorarray
CONFIG += testcase
TEMPLATE=app
QT += testlib qt3d
QT += opengl

CONFIG += warn_on

INCLUDEPATH += ../../../shared
SOURCES += tst_qvectorarray.cpp
