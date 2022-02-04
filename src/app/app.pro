QT += core
QT += gui
QT += quick
QT += concurrent
QT += widgets
#QT += serialbus
CONFIG += c++17
QT += gui-private

#DESTDIR = $$PWD/../../build/src/libs/pythonqt/lib
#message($$DESTDIR)



HEADERS += \
    engine.h \
    fpsmonitor.h \
    scene/componentsprovider.h \
    scene/domenmap.h \
    scene/kscene.h \
    scene/ksceneplayer.h \
    scene/ksceneprovider.h \
    sceneitems/qsg/background/kbackground.h \
    sceneitems/qsg/background/kgridbackground.h \
    sceneitems/qsg/background/kgridnode.h \
    sceneitems/qsg/background/kgridtext.h \
    tools/devlogger.h \
    tools/flexibleproxymodel.h \
    tools/objectservice.h \
    tools/pluginsmanager.h

SOURCES += \
    engine.cpp \
    main.cpp \
    fpsmonitor.cpp \
    scene/componentsprovider.cpp \
    scene/kscene.cpp \
    scene/domenmap.cpp \
    scene/ksceneplayer.cpp \
    scene/ksceneprovider.cpp \
    sceneitems/qsg/background/kgridnode.cpp \
    sceneitems/qsg/background/kgridtext.cpp \
    sceneitems/qsg/background/kbackground.cpp \
    sceneitems/qsg/background/kgridbackground.cpp \
    tools/devlogger.cpp \
    tools/flexibleproxymodel.cpp \
    tools/objectservice.cpp \
    tools/pluginsmanager.cpp


INCLUDEPATH += ./..

#linux-g++ {
#    QMAKE_CXXFLAGS_WARN_ON = -Wall -Wextra -Wno-enum-compare
#}

RESOURCES += \
    qrc.qrc

#QML_IMPORT_PATH += $$OUT_PWD/../../bin/plugins/
QML_IMPORT_PATH += $$PWD/../plugins/
DISTFILES +=

#================SPECIAL_PROPERTIES_STARTS================
CONFIG(release, debug|release){
DEFINES += "RELEASE"
DEFINES += QT_NO_DEBUG_OUTPUT
LIBS += -L$$PWD/../../bin/libs -lcore
LIBS += -L$$PWD/../../bin/libs -lpluginsbase
LIBS += -L$$PWD/../../bin/libs -lklibcorelite
LIBS += -L$$PWD/../../bin/libs -ldatabase
LIBS += -L$$PWD/../../bin/libs -lClientParser
LIBS += -L$$PWD/../../bin/libs -lTCPWorkers
LIBS += -L$$PWD/../../bin/libs -lAudio
LIBS += -L$$PWD/../../bin/libs -lAudioStreamingLibCore
LIBS += -L$$PWD/../../bin/libs -lTrajectory
DEFINES += QT_NO_DEBUG_OUTPUT
} else {
win32: LIBS += -L$$OUT_PWD/../libs/PluginsBase/debug -lpluginsbase
else:unix: LIBS += -L$$OUT_PWD/../libs/PluginsBase/ -lpluginsbase

win32: LIBS += -L$$OUT_PWD/../libs/core/debug -lcore
else:unix: LIBS += -L$$OUT_PWD/../libs/core/ -lcore

win32: LIBS += -L$$OUT_PWD/../libs/klibcorelite/debug -lklibcorelite
else:unix: LIBS += -L$$OUT_PWD/../libs/klibcorelite/ -lklibcorelite

win32: LIBS += -L$$OUT_PWD/../libs/database/debug -ldatabase
else:unix: LIBS += -L$$OUT_PWD/../libs/database/ -ldatabase

win32: LIBS += -L$$OUT_PWD/../libs/ClientParser/debug -lClientParser
else:unix: LIBS += -L$$OUT_PWD/../libs/ClientParser/ -lClientParser

win32: LIBS += -L$$OUT_PWD/../libs/TCPWorkers/debug -lTCPWorkers
else:unix: LIBS += -L$$OUT_PWD/../libs/TCPWorkers/ -lTCPWorkers

win32: LIBS += -L$$OUT_PWD/../libs/Audio/debug/ -lAudio
else:unix: LIBS += -L$$OUT_PWD/../libs/Audio/ -lAudio

win32: LIBS += -L$$OUT_PWD/../libs/AudioStreaming/AudioStreamingLib/AudioStreamingLibCore/debug/ -lAudioStreamingLibCore
else:unix: LIBS += -L$$OUT_PWD/../libs/AudioStreaming/AudioStreamingLib/AudioStreamingLibCore/ -lAudioStreamingLibCore

win32: LIBS += -L$$OUT_PWD/../libs/Trajectory/debug/ -lTrajectory
else:unix: LIBS += -L$$OUT_PWD/../libs/Trajectory/ -lTrajectory

}

INCLUDEPATH += $$PWD/../libs/PluginsBase
DEPENDPATH += $$PWD/../libs/PluginsBase

INCLUDEPATH += $$PWD/../libs/core
DEPENDPATH += $$PWD/../libs/core

INCLUDEPATH += $$PWD/../libs/klibcorelite
DEPENDPATH += $$PWD/../libs/klibcorelite

INCLUDEPATH += $$PWD/../libs/database
DEPENDPATH += $$PWD/../libs/database

INCLUDEPATH += $$PWD/../libs/ClientParser
DEPENDPATH += $$PWD/../libs/ClientParser

INCLUDEPATH += $$PWD/../libs/TCPWorkers
DEPENDPATH += $$PWD/../libs/TCPWorkers

INCLUDEPATH += $$PWD/../libs/Audio
DEPENDPATH += $$PWD/../libs/Audio

LIBS += -L$$PWD/../../bin/plugins/Trajectory/ -lTrajectory
INCLUDEPATH += $$PWD/../plugins/Trajectory
DEPENDPATH += $$PWD/../plugins/Trajectory

INCLUDEPATH += $$PWD/../libs/AudioStreaming/AudioStreamingLib/AudioStreamingLibCore
DEPENDPATH += $$PWD/../libs/AudioStreaming/AudioStreamingLib/AudioStreamingLibCore/

INCLUDEPATH += $$PWD/../libs/Trajectory
DEPENDPATH += $$PWD/../libs/Trajectory

include(../libs/pythonqt/build/common_libs.prf)
include(../libs/pythonqt/build/PythonQt_app.prf)
include(../libs/pythonqt/build/PythonQt_QtAll_app.prf)
#================SPECIAL_PROPERTIES_ENDS================




win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libs/kbus/release/ -lKBus
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libs/kbus/debug/ -lKBus
else:unix: LIBS += -L$$OUT_PWD/../libs/kbus/ -lKBus

INCLUDEPATH += $$PWD/../libs/kbus
DEPENDPATH += $$PWD/../libs/kbus

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libs/vdebug/release/ -lvdebug
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libs/vdebug/debug/ -lvdebug
else:unix: LIBS += -L$$OUT_PWD/../libs/vdebug/ -lvdebug

INCLUDEPATH += $$PWD/../libs/vdebug
DEPENDPATH += $$PWD/../libs/vdebug
