TEMPLATE = subdirs

src_exampleresources.subdir = $$PWD/exampleresources
src_exampleresources.target = sub-exampleresources

src_simpleqml.subdir = $$PWD/simple-qml
src_simpleqml.target = sub-simpleqml
src_simpleqml.depends = src_exampleresources

src_gltf.subdir = $$PWD/gltf
src_gltf.target = sub-gltf
src_gltf.depends = src_exampleresources

src_assimp.subdir = $$PWD/assimp
src_assimp.target = sub-assimp
src_assimp.depends = src_exampleresources

src_cppexample.subdir = $$PWD/cpp_example
src_cppexample.target = sub-cppexample
src_cppexample.depends = src_exampleresources

src_multiviewport.subdir = $$PWD/multiviewport
src_multiviewport.target = sub-multiviewport
src_multiviewport.depends = src_exampleresources

src_torusqml.subdir = $$PWD/torus-qml
src_torusqml.target = sub-torusqml
src_torusqml.depends = src_exampleresources

src_toruscpp.subdir = $$PWD/torus-cpp
src_toruscpp.target = sub-toruscpp
src_toruscpp.depends = src_exampleresources

src_bigmodelqml.subdir = $$PWD/bigmodel-qml
src_bigmodelqml.target = sub-bigmodelqml
src_bigmodelqml.depends = src_exampleresources

src_rollerball.subdir = $$PWD/rollerball
src_rollerball.target = sub-rollerball
src_rollerball.depends = src_exampleresources


SUBDIRS += \
    src_exampleresources \
    src_simpleqml \
    src_gltf \
    src_assimp \
    src_cppexample \
    src_multiviewport \
    src_torusqml \
    src_toruscpp \
    src_bigmodelqml

# TODO Port the old examples to new APIs
#SUBDIRS += qt3d
#qtHaveModule(qml): SUBDIRS += quick3d

qtHaveModule(3dbulletphysics) {
    SUBDIRS += src_rollerball
}
