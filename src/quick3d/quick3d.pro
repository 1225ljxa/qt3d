load(qt_module)

TARGET     = Qt3DQuick
MODULE     = 3dquick
CONFIG += module
QT         = core gui qml quick 3d

load(qt_module_config)

# private dependencies
QT += network

gcov {
    CONFIG += staticlib warn_on
    QMAKE_CXXFLAGS += -fprofile-arcs -ftest-coverage
    QMAKE_LFLAGS += -fprofile-arcs -ftest-coverage
} else {
    CONFIG += dll warn_on
}

# Use this define to set a data directory into which QML resources
# will be deployed and where qmlres.h will search for them
# If not defined this defaults to /usr/share/qt5/qt3d for Qt5 and
# to /usr/share/qt4/qt3d for Qt4.
#
# eg:    qmake CONFIG+=qt3d_qml_deploy_dir=/usr/local/share/qt3d
qt3d_qml_deploy_dir {
    DEFINES += QT3D_QML_DEPLOY_DIR=\\\"/usr/local/share/qt3d\\\"
}

include(quick3d.pri)

PUBLIC_HEADERS = $$HEADERS
HEADERS += $$PRIVATE_HEADERS
DEFINES += QT_BUILD_QT3D_QUICK_LIB

!contains(QT_CONFIG, egl):DEFINES += QT_NO_EGL






# ========== install additional files ==========

QML_FILES = \
    Cube.qml \
    Sphere.qml \
    Quad.qml \
    Teapot.qml \
    Capsule.qml \
    Cylinder.qml

QML_INFRA_FILES += \
    $$QML_FILES \
    qmldir \
    library.xml \
    plugins.qmltypes \
    cube.obj \
    quad.obj \
    teapot.bez
    # see the file README.library_xml for more on library.xml

package {
    copyqmlinfra_install.files = $$QML_INFRA_FILES
    copyqmlinfra_install.path = $$[QT_INSTALL_IMPORTS]/Qt3D/Shapes
    INSTALLS += copyqmlinfra_install
} else {
    copyqmlinfra.input = QML_INFRA_FILES
    copyqmlinfra.output = $$[QT_INSTALL_IMPORTS]/Qt3D/Shapes/${QMAKE_FILE_IN_BASE}${QMAKE_FILE_EXT}
    copyqmlinfra.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
    copyqmlinfra.CONFIG += no_link_no_clean
    copyqmlinfra.variable_out = PRE_TARGETDEPS
    QMAKE_EXTRA_COMPILERS += copyqmlinfra
}

OTHER_FILES += \
    README.plugins_types \
    README.library_xml

OTHER_FILES += $$QML_FILES
