TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS += \
    exampleresources \
    simple-qml \
    gltf \
    assimp \
    cpp_example \
    multiviewport \
    torus-qml \
    torus-cpp

# TODO Port the old examples to new APIs
#SUBDIRS += qt3d
#qtHaveModule(qml): SUBDIRS += quick3d
