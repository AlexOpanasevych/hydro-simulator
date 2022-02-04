TEMPLATE = lib
TARGET = AudioModule
QT += qml quick multimedia concurrent
CONFIG += qt plugin c++17 qmltypes

QML_IMPORT_NAME = AudioModule
QML_IMPORT_MAJOR_VERSION = 1.0
DESTDIR = $$PWD/../../../bin/plugins/$$QML_IMPORT_NAME
uri = AudioModule
QMLTYPES_FILENAME = $$DESTDIR/AudioModule.qmltypes

# Input
SOURCES += \
        audiomodule_plugin.cpp \
        audiomodule.cpp

HEADERS += \
        audiomodule_plugin.h \
        audiomodule.h

DISTFILES = qmldir \
    qml/AudioModuleSettingsButton.qml \
    qml/AudioModuleSettingsWindow.qml


CONFIG(release, debug|release){
INCLUDEPATH += $$PWD/../../
DEFINES += RELEASE
LIBS += -L$$PWD/../../../bin/libs/ -lcore
LIBS += -L$$PWD/../../../bin/libs/ -lklibcorelite
LIBS += -L$$PWD/../../../bin/libs/ -lpluginsbase
LIBS += -L$$PWD/../../../bin/libs/ -lAudio
LIBS += -L$$PWD/../../../bin/libs/AudioStreaming/AudioStreamingLib/AudioStreamingLibCore/ -lAudioStreamingLibCore

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

win32: LIBS += -L$$OUT_PWD/../../libs/AudioStreaming/AudioStreamingLib/AudioStreamingLibCore/debug/ -lAudioStreamingLibCore
else:unix: LIBS += -L$$OUT_PWD/../../libs/AudioStreaming/AudioStreamingLib/AudioStreamingLibCore/ -lAudioStreamingLibCore
}
INCLUDEPATH += $$PWD/../../libs/PluginsBase
DEPENDPATH += $$PWD/../../libs/PluginsBase

INCLUDEPATH += $$PWD/../../libs/Audio
DEPENDPATH += $$PWD/../../libs/Audio

INCLUDEPATH += $$PWD/../../libs/klibcorelite
DEPENDPATH += $$PWD/../../libs/klibcorelite

INCLUDEPATH += $$PWD/../../libs/core
DEPENDPATH += $$PWD/../../libs/core

INCLUDEPATH += $$PWD/../../libs/AudioStreaming/AudioStreamingLib/AudioStreamingLibCore
DEPENDPATH += $$PWD/../../libs/AudioStreaming/AudioStreamingLib/AudioStreamingLibCore


#=============QMLDIR_COPY=============#
!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$DESTDIR/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) "$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)" "$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
#=============COPIES_STARTS=============#

default_copy.files = \
     qmldir \
     depencies.json
default_copy.path = $$DESTDIR/

qml_copy.files = \
     qml/AudioModuleSettingsWindow.qml \
     qml/AudioModuleSettingsButton.qml
qml_copy.path = $$DESTDIR/qml/

sounds_copy.files = \
    sounds/atom_sound.sd \
    sounds/engine_sound.sd \
    sounds/engine_sound2.sd \
    sounds/Miss_Rager.sd \
    sounds/noise.sd \
    sounds/noisego.sd \
    sounds/parcel12.sd \
    sounds/reverberation.sd \
    sounds/shipSound.sd \
    sounds/wave_radiation.sd
sounds_copy.path = $$DESTDIR/sounds/

COPIES += default_copy qml_copy sounds_copy
#=============COPIES_END=============#




