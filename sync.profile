%modules = ( # path to module name map
    "Qt3DCore" => "$basedir/src/core",
    "Qt3DRenderer" => "$basedir/src/render",
    "Qt3DQuick" => "$basedir/src/quick3d/quick3d",
    "Qt3DQuickRenderer" => "$basedir/src/quick3d/quick3drenderer",
    "Qt3DOpenAL" => "$basedir/src/openal",
    "Qt3DBulletPhysics" => "$basedir/src/bulletphysics",
    "Qt3DQuickBulletPhysics" => "$basedir/src/quick3d/quick3dbulletphysics",
    "Qt3DInput" => "$basedir/src/input",
);
%moduleheaders = ( # restrict the module headers to those found in relative path
);

# Temporary hack to force generation of camel case headers until we can refactor
# to get QBlah class names. This is also affected by the Qt3D namespace which we need
# to avoid naming collisions with the likes of QTransform
$publicclassregexp = "Qt3D::.+";

# Module dependencies.
# Every module that is required to build this module should have one entry.
# Each of the module version specifiers can take one of the following values:
#   - A specific Git revision.
#   - any git symbolic ref resolvable from the module's repository (e.g. "refs/heads/master" to track master branch)
#
%dependencies = (
    "qtbase" => "refs/heads/dev",
    "qtxmlpatterns" => "refs/heads/dev",
    "qtdeclarative" => "refs/heads/dev",
);
