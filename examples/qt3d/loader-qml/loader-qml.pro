TEMPLATE = app
QT += 3dcore 3drenderer 3dinput 3dquick qml quick

SOURCES += \
    main.cpp

OTHER_FILES += \
    main.qml \
    SphereEntity.qml \
    CuboidEntity.qml

RESOURCES += \
    loader-qml.qrc
