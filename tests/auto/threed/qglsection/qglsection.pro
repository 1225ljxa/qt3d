TARGET = tst_qglsection
CONFIG += testcase
TEMPLATE=app
QT += testlib qt3d
CONFIG += warn_on

INCLUDEPATH += ../../../shared
SOURCES += tst_qglsection.cpp
INCLUDEPATH += ../../../../src/threed/geometry
