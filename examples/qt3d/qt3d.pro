TEMPLATE = subdirs

SUBDIRS += \
    exampleresources \
    playground-qml \
    simple-qml \
    simple-cpp \
    gltf \
    assimp \
    cpp_example \
    multiviewport \
    torus-qml \
    torus-cpp \
    cylinder-qml \
    cylinder-cpp \
    bigmodel-qml \
    wireframe \
    tessellation-modes \
    deferred-renderer-qml \
    deferred-renderer-cpp \
    shadow-map-qml \
    bigscene-cpp \
    materials \
    keyboardinput-qml \
    loader-qml \
    wave \
    materials-cpp \
    dynamicscene-cpp \
    scene3d \
    enabled-qml \
    skybox \
    controls \
    anaglyph-rendering

# TODO Port the old examples to new APIs
#SUBDIRS += qt3d
#qtHaveModule(qml): SUBDIRS += quick3d

# Make all other subdirs depend on exampleresources
for(subdir, SUBDIRS) {
    !equals(subdir, exampleresources) {
        $${subdir}.depends += exampleresources
    }
}

