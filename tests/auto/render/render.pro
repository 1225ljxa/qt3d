TEMPLATE = subdirs

contains(QT_CONFIG, private_tests) {
    SUBDIRS += \
        renderentity \
        renderqueue \
        renderrenderpass \
        qgraphicsutils \
        rendershader \
        renderviewutils \
        renderviews \
        rendermaterial \
        rendermesh \
        vsyncframeadvanceservice
}
