#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = Vessel
QT += qml
QT += quick
CONFIG += qt
CONFIG += plugin
CONFIG += c++17
CONFIG += qmltypes
CONFIG += shared dll

QML_IMPORT_NAME = Vessel
QML_IMPORT_MAJOR_VERSION = 1.0
DESTDIR = $$OUT_PWD/../../../bin/plugins/$$QML_IMPORT_NAME
QMLTYPES_FILENAME = $$DESTDIR/Vessel.qmltypes
uri = Vessel

#Inputs
DEFINES += VESSEL_LIBRARY
SOURCES += \
        #engine/controllers/torpedocontroller.cpp \
        engine/controllers/vesselcontroller.cpp \
        #engine/controllers/wavecontroller.cpp \
        #engine/messages/addsegmentmessage.cpp \
        #engine/messages/changeanglemsg.cpp \
        #engine/messages/changecoordsmsg.cpp \
        #engine/messages/changevelocitymsg.cpp \
        #engine/messages/setshippathmsg.cpp \
        #engine/messages/settorpedoparams.cpp \
        #engine/sampledata/torpedocontrollersampledata.cpp \
        #engine/sampledata/torpedosampledata.cpp \
        engine/sampledata/vesselcontrollersampledata.cpp \
        engine/sampledata/vesselsampledata.cpp \
        #engine/sampledata/wave.cpp \
        physicalitems_plugin.cpp \
        #sceneItems/trace/trace.cpp \
        #sceneItems/trace/segmentdata.cpp \
        #sceneItems/trace/trajectory.cpp \
        sceneItems/vessel.cpp #\
        #sceneItems/torpedo.cpp #\
        #qsgItems/traceitem.cpp \
        #qsgItems/segmentitem.cpp \
        #qsgItems/trajectoryitem.cpp


HEADERS += \
        #engine/controllers/torpedocontroller.h \
        engine/controllers/vesselcontroller.h \
        #engine/controllers/wavecontroller.h \
        #engine/messages/addsegmentmessage.h \
        #engine/messages/changeanglemsg.h \
        #engine/messages/changecoordsmsg.h \
        #engine/messages/changevelocitymsg.h \
        #engine/messages/setshippathmsg.h \
        #engine/messages/settorpedoparams.h \
        #engine/sampledata/torpedocontrollersampledata.h \
        #engine/sampledata/torpedosampledata.h \
        engine/sampledata/vesselcontrollersampledata.h \
        engine/sampledata/vesselsampledata.h \
        #engine/sampledata/wave.h \
        #qsgItems/segmentitem.h \
        #qsgItems/traceitem.h \
        #qsgItems/trajectoryitem.h \
        #sceneItems/trace/trace.h \
        #sceneItems/trace/segmentdata.h \
        #sceneItems/trace/trajectory.h \
        physicalitems_global.h \
        physicalitems_plugin.h \
        sceneItems/vessel.h #\
        #sceneItems/torpedo.h

DISTFILES = \ 
        #qml/menuButton/TorpedoAddItemMenuButton.qml \
        qml/menuButton/VesselAddItemMenuButton.qml \
        #qml/menuWindow/TorpedoAddItemMenuWindow.qml \
        qml/menuWindow/VesselAddItemMenuWindow.qml \
        #qml/sceneLoader/TorpedoSceneLoader.qml \
        qml/sceneLoader/VesselSceneLoader.qml \
        qmldir \
        #qml/items/TorpedoItem.qml \
        qml/items/VesselItem.qml \
        qml/menuButton/VesselMenuButton.qml \
        #qml/menuButton/TorpedoMenuButton.qml \
        qml/menuWindow/VesselMenuWindow.qml \
        #qml/menuWindow/TorpedoMenuWindow.qml \
        #qml/properties/TorpedoProperties.qml \
        qml/properties/VesselProperties.qml

QML_IMPORT_PATH += $$OUT_PWD/../../../bin/plugins

#=============QMLDIR_COPY=============#
!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$DESTDIR/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) "$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)" "$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

#================SPECIAL_PROPERTIES_STARTS================
CONFIG(release, debug|release){
DEFINES += RELEASE
INCLUDEPATH += $$PWD/../../
LIBS += -L$$OUT_PWD/../../../bin/libs -lcore
LIBS += -L$$OUT_PWD/../bin/libs -lklibcorelite
LIBS += -L$$OUT_PWD/../bin/libs -lpluginsbase

}else{
INCLUDEPATH += $$PWD/../../

win32: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/debug -lpluginsbase
else:unix: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/ -lpluginsbase

win32: LIBS += -L$$OUT_PWD/../../libs/core/debug -lcore
else:unix: LIBS += -L$$OUT_PWD/../../libs/core/ -lcore

win32: LIBS += -L$$OUT_PWD/../../libs/klibcorelite/debug/ -lklibcorelite
else:unix: LIBS += -L$$OUT_PWD/../../libs/klibcorelite/ -lklibcorelite
}

INCLUDEPATH += $$PWD/../../libs/core
DEPENDPATH += $$PWD/../../libs/core

INCLUDEPATH += $$PWD/../../libs/klibcorelite
DEPENDPATH += $$PWD/../../libs/klibcorelite

INCLUDEPATH += $$PWD/../../libs/PluginsBase
DEPENDPATH += $$PWD/../../libs/PluginsBase

LIBS += -L$$OUT_PWD/../../../bin/plugins/Trajectory -lTrajectory
INCLUDEPATH += $$PWD/../Trajectory
DEPENDPATH += $$PWD/../Trajectory

#================SPECIAL_PROPERTIES_ENDS================

#=============COPIES_STARTS=============#
qmldir.files = qmldir

default_copy.files = \ 
     qmldir
default_copy.path = $$DESTDIR/

qml_items_copy.files = \ 
     qml/items/VesselItem.qml \ 
     qml/items/TorpedoItem.qml
qml_items_copy.path = $$DESTDIR/qml/items/

qml_menuButton_copy.files = \ 
     qml/menuButton/TorpedoMenuButton.qml \ 
     qml/menuButton/VesselMenuButton.qml \ 
     qml/menuButton/VesselAddItemMenuButton.qml \ 
     qml/menuButton/TorpedoAddItemMenuButton.qml
qml_menuButton_copy.path = $$DESTDIR/qml/menuButton/

qml_menuWindow_copy.files = \ 
     qml/menuWindow/TorpedoMenuWindow.qml \ 
     qml/menuWindow/VesselMenuWindow.qml \ 
     qml/menuWindow/VesselAddItemMenuWindow.qml \ 
     qml/menuWindow/TorpedoAddItemMenuWindow.qml
qml_menuWindow_copy.path = $$DESTDIR/qml/menuWindow/

qml_properties_copy.files = \ 
     qml/properties/VesselProperties.qml \ 
     qml/properties/TorpedoProperties.qml
qml_properties_copy.path = $$DESTDIR/qml/properties/

qml_sceneLoader_copy.files = \ 
     qml/sceneLoader/VesselSceneLoader.qml \ 
     qml/sceneLoader/TorpedoSceneLoader.qml
qml_sceneLoader_copy.path = $$DESTDIR/qml/sceneLoader/

COPIES += default_copy qml_items_copy qml_menuButton_copy qml_menuWindow_copy qml_properties_copy qml_sceneLoader_copy
#=============COPIES_END=============#
