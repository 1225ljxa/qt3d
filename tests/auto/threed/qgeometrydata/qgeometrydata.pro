TARGET = tst_qgeometrydata
CONFIG += testcase
TEMPLATE=app
QT += testlib qt3d
CONFIG += warn_on

INCLUDEPATH += ../../../shared
INCLUDEPATH += ../../../../src/threed/geometry

SOURCES += tst_qgeometrydata.cpp

CONFIG+=insignificant_test
