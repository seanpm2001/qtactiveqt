TEMPLATE      = subdirs
SUBDIRS      += comapp \
                menus \
                multiple \
                simple \
                wrapper

contains(QT_CONFIG, opengl):!contains(QT_CONFIG, opengles2): SUBDIRS += opengl
qtHaveModule(quickcontrols2):SUBDIRS += simpleqml
