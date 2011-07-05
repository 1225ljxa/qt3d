TEMPLATE = lib
TARGET  = qthreedqmlplugin
CONFIG += qt plugin

include(../../../qt3d_pkg_dep.pri)
include(../../../qt3dquick_pkg_dep.pri)

# See the README in the root dir re this code
package {
    target.path += $$QT3D_INSTALL_IMPORTS/Qt3D
    INSTALLS += target
} else {
    CONFIG += qt3dquick qt3d
    DESTDIR = $$QT3D_INSTALL_IMPORTS/Qt3D
}

win32 {
    CONFIG(debug, debug|release) {
        TARGET = $$member(TARGET, 0)d
    }
}

symbian {
    CONFIG += epocallowdlldata
    MMP_RULES += EXPORTUNFROZEN
    LIBS += -lQt3D -lQt3DQuick
    TARGET.CAPABILITY = All -Tcb
    TARGET.UID3 = 0x20031E9B
}

SOURCES += \
    threed.cpp \
    qt3dnamespace.cpp \
    scale3d.cpp \
    viewport.cpp \
    qgraphicslookattransform.cpp \
    shaderprogram.cpp

HEADERS += \
    qt3dnamespace.h \
    scale3d.h \
    viewport.h \
    qgraphicslookattransform.h \
    shaderprogram.h \
    shaderprogram_p.h

# See the file README.library_xml for more on this
qdeclarativesources.files += \
    qmldir \
    library.xml \
    plugins.qmltypes

!package {
    qdeclarative_in_place.input = qdeclarativesources.files
    qdeclarative_in_place.output = $$QT3D_INSTALL_IMPORTS/Qt3D/${QMAKE_FILE_IN_BASE}${QMAKE_FILE_EXT}
    qdeclarative_in_place.commands = $$QMAKE_COPY ${QMAKE_FILE_IN} ${QMAKE_FILE_OUT}
    qdeclarative_in_place.CONFIG += no_link_no_clean
    qdeclarative_in_place.variable_out = PRE_TARGETDEPS
    QMAKE_EXTRA_COMPILERS += qdeclarative_in_place
} else {
    qdeclarativesources.path += $$QT3D_INSTALL_IMPORTS/Qt3D
    INSTALLS += qdeclarativesources
}

OTHER_FILES += \
    README.plugins_types \
    README.library_xml
