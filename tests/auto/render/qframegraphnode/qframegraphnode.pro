TEMPLATE = app

TARGET = tst_qframegraphnode
QT += core-private 3dcore 3dcore-private 3drenderer 3drenderer-private testlib

CONFIG += testcase

SOURCES += tst_qframegraphnode.cpp

include(../commons/commons.pri)
