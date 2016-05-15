TEMPLATE = subdirs

SUBDIRS += \
    simple-qml \
    simple-cpp \
    multiviewport \
    wireframe \
    tessellation-modes \
    shadow-map-qml \
    materials \
    wave \
    materials-cpp \
    scene3d \
    skybox \
    controls \
    anaglyph-rendering \
    simple-shaders-qml \
    planets-qml \
    transparency-qml \
    transparency-qml-scene3d \
    scene3d-loader \
    instanced-arrays-qml \
    transforms-qml \
    lights \
    compute-particles

qtHaveModule(multimedia): SUBDIRS += audio-visualizer-qml

# qmake seems to break in some CI configurations, disable this for now
#SUBDIRS += qgltf

# TODO Port the old examples to new APIs
#SUBDIRS += qt3d

qtHaveModule(widgets): SUBDIRS += assimp-cpp \
    basicshapes-cpp
