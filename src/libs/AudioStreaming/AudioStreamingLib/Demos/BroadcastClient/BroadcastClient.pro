QT +=       core gui widgets multimedia concurrent

TARGET =    BroadcastClient
TEMPLATE =  app


SOURCES +=  main.cpp \
            mainwindow.cpp \
            levelwidget.cpp \
            barswidget.cpp \
            spectrumanalyzer.cpp \
            waveformwidget.cpp \
            audiorecorder.cpp \
            mp3recorder.cpp

HEADERS  += mainwindow.h \
            common.h \
            levelwidget.h \
            barswidget.h \
            spectrumanalyzer.h \
            waveformwidget.h \
            audiorecorder.h \
            mp3recorder.h

INCLUDEPATH += $$PWD/3rdparty/kiss_fft130
SOURCES += $$PWD/3rdparty/kiss_fft130/kiss_fft.c

INCLUDEPATH += $$PWD/3rdparty/lame/include

include(../../AudioStreamingLib.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../AudioStreamingLibCore/release/ -lAudioStreamingLibCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../AudioStreamingLibCore/debug/ -lAudioStreamingLibCore
else:unix: LIBS += -L$$OUT_PWD/../../AudioStreamingLibCore/ -lAudioStreamingLibCore

INCLUDEPATH += $$PWD/../../AudioStreamingLibCore
DEPENDPATH += $$PWD/../../AudioStreamingLibCore
