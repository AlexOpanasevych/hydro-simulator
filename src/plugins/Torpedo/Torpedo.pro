TEMPLATE = lib
TARGET = Torpedo
QT += qml
QT += quick
CONFIG += qt
CONFIG += plugin
CONFIG += c++17
CONFIG += qmltypes
CONFIG += shared dll

QML_IMPORT_NAME = Torpedo
QML_IMPORT_MAJOR_VERSION = 1.0
DESTDIR = $$PWD/../../../bin/plugins/$$QML_IMPORT_NAME
QMLTYPES_FILENAME = $$DESTDIR/Torpedo.qmltypes
uri = Torpedo
DEFINES += TORPEDO_LIBRARY
# Input
SOURCES += \
        engine/controllers/torpedocontroller.cpp \
        engine/sampledata/torpedocontrollersampledata.cpp \
        engine/sampledata/torpedosampledata.cpp \
        sceneItems/torpedo.cpp \
        torpedo_plugin.cpp

HEADERS += \
        engine/controllers/torpedocontroller.h \
        engine/sampledata/torpedocontrollersampledata.h \
        engine/sampledata/torpedosampledata.h \
        sceneItems/torpedo.h \
        torpedo_global.h \
        torpedo_plugin.h

DISTFILES = qmldir \
        qml/items/TorpedoItem.qml \
        qml/items/ViewField.qml \
        qml/menuButton/TorpedoAddItemMenuButton.qml \
        qml/menuButton/TorpedoMenuButton.qml \
        qml/menuWindow/TorpedoAddItemMenuWindow.qml \
        qml/menuWindow/TorpedoMenuWindow.qml \
        qml/properties/TorpedoProperties.qml \
        qml/sceneLoader/TorpedoSceneLoader.qml

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$DESTDIR/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) "$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)" "$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir

CONFIG(release, debug|release){
DEFINES += RELEASE
INCLUDEPATH += $$PWD/../../
LIBS += -L$$PWD/../../../bin/libs -lcore
LIBS += -L$$PWD/../../../bin/libs -lklibcorelite
LIBS += -L$$PWD/../../../bin/libs -lpluginsbase
LIBS += -L$$PWD/../../../bin/libs -lTrajectory
}else{
INCLUDEPATH += $$PWD/../../

win32: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/debug -lpluginsbase
else:unix: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/ -lpluginsbase

win32: LIBS += -L$$OUT_PWD/../../libs/core/debug -lcore
else:unix: LIBS += -L$$OUT_PWD/../../libs/core/ -lcore

win32: LIBS += -L$$OUT_PWD/../../libs/klibcorelite/debug/ -lklibcorelite
else:unix: LIBS += -L$$OUT_PWD/../../libs/klibcorelite/ -lklibcorelite

win32:LIBS += -L$$OUT_PWD/../../libs/Trajectory/debug/ -lTrajectory
else:unix: LIBS += -L$$OUT_PWD/../../libs/Trajectory/ -lTrajectory
}
QML_IMPORT_PATH += $$PWD/../../../bin/plugins/

INCLUDEPATH += $$PWD/../../libs/core
DEPENDPATH += $$PWD/../../libs/core

INCLUDEPATH += $$PWD/../../libs/klibcorelite
DEPENDPATH += $$PWD/../../libs/klibcorelite

INCLUDEPATH += $$PWD/../../libs/PluginsBase
DEPENDPATH += $$PWD/../../libs/PluginsBase

INCLUDEPATH += $$PWD/../../libs/Trajectory
DEPENDPATH += $$PWD/../../libs/Trajectory

#=============COPIES_STARTS=============#
default_copy.files = \
     qmldir \
     depencies.json
default_copy.path = $$DESTDIR/

qml_items_copy.files = \
     qml/items/TorpedoItem.qml \
     qml/items/ViewField.qml
qml_items_copy.path = $$DESTDIR/qml/items/

qml_menuButton_copy.files = \
     qml/menuButton/TorpedoMenuButton.qml \
     qml/menuButton/TorpedoAddItemMenuButton.qml
qml_menuButton_copy.path = $$DESTDIR/qml/menuButton/

qml_menuWindow_copy.files = \
     qml/menuWindow/TorpedoMenuWindow.qml \
     qml/menuWindow/TorpedoAddItemMenuWindow.qml
qml_menuWindow_copy.path = $$DESTDIR/qml/menuWindow/

qml_properties_copy.files = \
     qml/properties/TorpedoProperties.qml
qml_properties_copy.path = $$DESTDIR/qml/properties/

qml_sceneLoader_copy.files = \
     qml/sceneLoader/TorpedoSceneLoader.qml
qml_sceneLoader_copy.path = $$DESTDIR/qml/sceneLoader/

COPIES += default_copy qml_items_copy qml_menuButton_copy qml_menuWindow_copy qml_properties_copy qml_sceneLoader_copy
#=============COPIES_END=============#

