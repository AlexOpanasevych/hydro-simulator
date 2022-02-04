#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = Vessel
QT += qml
QT += quick
QT += concurrent
CONFIG += qt
CONFIG += plugin
CONFIG += c++17
CONFIG += qmltypes

QML_IMPORT_NAME = Vessel
QML_IMPORT_MAJOR_VERSION = 1.0

DESTDIR = $$PWD/../../../bin/plugins/$$QML_IMPORT_NAME
uri = Vessel
QMLTYPES_FILENAME = $$DESTDIR/Vessel.qmltypes

#Inputs
DEFINES += VESSEL_LIBRARY
SOURCES += \
        engine/controllers/vesselcontroller.cpp \
        engine/messages/changeanglemsg.cpp \
        engine/messages/changecoordsmsg.cpp \
        engine/messages/changevelocitymsg.cpp \
        engine/messages/setshippathmsg.cpp \
        engine/sampledata/vesselcontrollersampledata.cpp \
        engine/sampledata/vesselsampledata.cpp \
        sceneItems/vessel.cpp \
        vessel_plugin.cpp

HEADERS += \
        engine/controllers/vesselcontroller.h \
        engine/messages/changeanglemsg.h \
        engine/messages/changecoordsmsg.h \
        engine/messages/changevelocitymsg.h \
        engine/messages/setshippathmsg.h \
        engine/sampledata/vesselcontrollersampledata.h \
        engine/sampledata/vesselsampledata.h \
        vessel_global.h \
        sceneItems/vessel.h \
        vessel_plugin.h


DISTFILES = \
    qml/sceneLoader/ExtraInfoPanel.qml \
    qml/sceneLoader/VesselSceneLoader.qml \
    qmldir \
    qml/menuButton/VesselMenuButton.qml \ 
    qml/menuWindow/VesselMenuWindow.qml \ 
    qml/menuButton/VesselAddItemMenuButton.qml \
    qml/menuWindow/VesselAddItemMenuWindow.qml \
    qml/properties/VesselProperties.qml  \
    qml/items/VesselItem.qml


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

win32: LIBS += -L$$OUT_PWD/../../libs/Trajectory/debug/ -lTrajectory
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

#=============QMLDIR_COPY=============#
!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$DESTDIR/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) "$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)" "$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

#=============COPIES_STARTS=============#
default_copy.files = \ 
     qmldir \
     depencies.json
default_copy.path = $$DESTDIR/

qml_items_copy.files = \ 
     qml/items/VesselItem.qml
qml_items_copy.path = $$DESTDIR/qml/items/

qml_menuButton_copy.files = \
     qml/menuButton/VesselMenuButton.qml \
     qml/menuButton/VesselAddItemMenuButton.qml
qml_menuButton_copy.path = $$DESTDIR/qml/menuButton/

qml_menuWindow_copy.files = \
     qml/menuWindow/VesselMenuWindow.qml \
     qml/menuWindow/VesselAddItemMenuWindow.qml
qml_menuWindow_copy.path = $$DESTDIR/qml/menuWindow/

qml_properties_copy.files = \ 
     qml/properties/VesselProperties.qml
qml_properties_copy.path = $$DESTDIR/qml/properties/

qml_sceneLoader_copy.files = \
     qml/sceneLoader/VesselSceneLoader.qml \
     qml/sceneLoader/ExtraInfoPanel.qml
qml_sceneLoader_copy.path = $$DESTDIR/qml/sceneLoader/

COPIES += default_copy qml_items_copy qml_menuButton_copy qml_menuWindow_copy qml_properties_copy qml_sceneLoader_copy
#=============COPIES_END=============#

RESOURCES += \
    resources.qrc

