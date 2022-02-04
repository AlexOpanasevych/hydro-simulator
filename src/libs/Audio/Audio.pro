#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = Audio
QT -= gui
QT += quick
QT += multimedia
QT += concurrent
CONFIG += shared dll
CONFIG += c++17

CONFIG(release, debug|release){
DESTDIR = $$PWD/../../../bin/libs/
}
#Inputs
DEFINES += AUDIO_LIBRARY
INCLUDEPATH += $$PWD/
INCLUDEPATH += $$PWD/../..
SOURCES += \
    audio.cpp \
    devices/sounddevice.cpp \
#    ksoundarray.cpp \
#    ksoundmix.cpp \
#    rawsound.cpp \
#    rootsound.cpp \
#    sinsound.cpp \
#    sounddevice.cpp \
    sound.cpp \
    sounds/afsound.cpp \
    sounds/kintegratedsound.cpp \
    sounds/rawsound.cpp \
    sounds/sinsound.cpp \
    sounds/soundfactory.cpp \
    utility/kamplitudetransformer.cpp \
    utility/kintegrator.cpp \
    utility/ksoundarray.cpp \
    utility/ksoundchain.cpp \
    utility/ksoundmix.cpp \
    utility/pauseelapsedtimer.cpp \
#    utility/qwinloopback.cpp


HEADERS += \
    Audio_global.h \
    audio.h \
    devices/sounddevice.h \
#    ksoundarray.h \
#    ksoundmix.h \
#    rawsound.h \
#    rootsound.h \
#    sinsound.h \
#    sounddevice.h \
    sound.h \
    sounds/afsound.h \
    sounds/kintegratedsound.h \
    sounds/rawsound.h \
    sounds/sinsound.h \
    sounds/soundfactory.h \
    utility/kamplitudetransformer.h \
    utility/kintegrator.h \
    utility/ksoundarray.h \
    utility/ksoundchain.h \
    utility/ksoundmix.h \
    utility/pauseelapsedtimer.h \
#    utility/qwinloopback.h

win32 {
SOURCES += utility/qwinloopback.cpp
HEADERS += utility/qwinloopback.h
LIBS += -lOle32 -lwinmm -lksuser -luuid
}

CONFIG(release, debug|release) {
DEFINES += RELEASE
    LIBS += -L$$PWD/../../../bin/libs/ -lklibcorelite
    LIBS += -L$$PWD/../../../bin/libs/ -lAudioStreamingLibCore
}
else {
    win32: LIBS += -L$$OUT_PWD/../klibcorelite/debug/ -lklibcorelite
    else:unix: LIBS += -L$$OUT_PWD/../klibcorelite/ -lklibcorelite

    win32: LIBS += -L$$OUT_PWD/../AudioStreaming/AudioStreamingLib/AudioStreamingLibCore/debug/ -lAudioStreamingLibCore
    else:unix: LIBS += -L$$OUT_PWD/../AudioStreaming/AudioStreamingLib/AudioStreamingLibCore/ -lAudioStreamingLibCore
}

INCLUDEPATH += $$PWD/../klibcorelite
DEPENDPATH += $$PWD/../klibcorelite

INCLUDEPATH += $$PWD/../AudioStreaming/AudioStreamingLib/AudioStreamingLibCore
DEPENDPATH += $$PWD/../AudioStreaming/AudioStreamingLib/AudioStreamingLibCore
