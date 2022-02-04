#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = Wave
QT += qml quick concurrent
CONFIG += plugin c++17 qmltypes qt

QML_IMPORT_NAME = Wave

QML_IMPORT_MAJOR_VERSION = 1.0

DESTDIR = $$PWD/../../../bin/plugins/$$QML_IMPORT_NAME

QMLTYPES_FILENAME = $$DESTDIR/Wave.qmltypes

uri = Wave

#Inputs
SOURCES += \
        engine/controllers/wavecontroller.cpp \
        wave_plugin.cpp \
        wavegenerator.cpp \
        waveitem.cpp \
        waveservice.cpp


HEADERS += \
        engine/controllers/wavecontroller.h \
        wave_plugin.h \
        wavegenerator.h \
        waveitem.h \
        waveservice.h


DISTFILES = qmldir 


CONFIG(release, debug|release){
DEFINES += RELEASE
INCLUDEPATH += $$PWD/../../
LIBS += -L$$PWD/../../../bin/libs -lcore
LIBS += -L$$PWD/../../../bin/libs -lklibcorelite
LIBS += -L$$PWD/../../../bin/libs -lpluginsbase
LIBS += -L$$PWD/../../../bin/libs -lAudio
}else{
INCLUDEPATH += $$PWD/../../

win32: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/debug -lpluginsbase
else:unix: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/ -lpluginsbase

win32: LIBS += -L$$OUT_PWD/../../libs/core/debug -lcore
else:unix: LIBS += -L$$OUT_PWD/../../libs/core/ -lcore

win32: LIBS += -L$$OUT_PWD/../../libs/klibcorelite/debug/ -lklibcorelite
else:unix: LIBS += -L$$OUT_PWD/../../libs/klibcorelite/ -lklibcorelite

win32: LIBS += -L$$OUT_PWD/../../libs/Audio/debug/ -lAudio
else:unix: LIBS += -L$$OUT_PWD/../../libs/Audio/ -lAudio
}

QML_IMPORT_PATH += $$PWD/../../../bin/plugins/

INCLUDEPATH += $$PWD/../../libs/core
DEPENDPATH += $$PWD/../../libs/core

INCLUDEPATH += $$PWD/../../libs/klibcorelite
DEPENDPATH += $$PWD/../../libs/klibcorelite

INCLUDEPATH += $$PWD/../../libs/PluginsBase
DEPENDPATH += $$PWD/../../libs/PluginsBase

INCLUDEPATH += $$PWD/../../libs/Audio
DEPENDPATH += $$PWD/../../libs/Audio

#=============QMLDIR_COPY=============#
!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$DESTDIR/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) "$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)" "$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}


#=============COPIES_STARTS=============#
qmldir.files = qmldir

default_copy.files = \ 
     qmldir \
     depencies.json
default_copy.path = $$DESTDIR/

COPIES += default_copy
#=============COPIES_END=============#
