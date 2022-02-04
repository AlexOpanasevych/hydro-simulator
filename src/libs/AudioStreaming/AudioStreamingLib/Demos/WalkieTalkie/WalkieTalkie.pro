QT +=       core gui widgets qml uitools

TARGET =    WalkieTalkie
TEMPLATE =  app


SOURCES +=  main.cpp \
            mainwindow.cpp \
            settings.cpp \
            levelwidget.cpp \
            audiorecorder.cpp \
            mp3recorder.cpp

HEADERS +=  mainwindow.h \
            common.h \
            settings.h \
            levelwidget.h \
            audiorecorder.h \
            mp3recorder.h

INCLUDEPATH += $$PWD/3rdparty/lame/include

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../AudioStreamingLibCore/release/ -lAudioStreamingLibCore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../AudioStreamingLibCore/debug/ -lAudioStreamingLibCore
else:unix: LIBS += -L$$OUT_PWD/../../AudioStreamingLibCore/ -lAudioStreamingLibCore

INCLUDEPATH += $$PWD/../../AudioStreamingLibCore
DEPENDPATH += $$PWD/../../AudioStreamingLibCore
