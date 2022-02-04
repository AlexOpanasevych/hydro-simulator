QT += quick sql gui-private gui multimedia

CONFIG += c++17
#include(../libs/AudioStreaming/AudioStreamingLib/AudioStreamingLib.pri)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        app.cpp \
        clientservice.cpp \
        clientsideparser.cpp \
        filter.cpp \
        flexibleproxymodel.cpp \
#        items/ImageSpLoader.cpp \
#        items/ImageSpNode.cpp \
        items/customitem.cpp \
        items/discretegraphdrawer.cpp \
        items/mgkabstractmonitor.cpp \
        items/mgknumberviewprivate.cpp \
        items/monitors/mgkbronzerectmonitor.cpp \
        items/monitors/mgkcirclemonitor.cpp \
        items/painters/bronzecsnrpainter.cpp \
        items/painters/bronzesweeppainter.cpp \
        items/painters/bronzetucpainter.cpp \
        items/painters/csnrpainter.cpp \
        items/mgkrecorderprivate.cpp \
        items/monitors/mgkrectmonitorprivate.cpp \
        items/painters/doplerpainter.cpp \
        items/painters/epo160painter.cpp \
        items/painters/epo1painter.cpp \
        items/painters/monitorpainter.cpp \
        items/painters/sweeppainter.cpp \
        items/painters/tucpainter.cpp \
        main.cpp \
        mgkitembase.cpp \
        mgkitemsfactory.cpp \
        simdatamanager.cpp \
        statecommunicator.cpp \
        updateservice.cpp \
        utility/amplifier.cpp \
        utility/delaynoisegenerator.cpp \
        utility/kdiscretizer.cpp \
        utility/kintegrator.cpp \
        utility/krandomstep.cpp \
        utility/krangedistribution.cpp \
        utility/kvectordiscretizer.cpp \
        utility/noisegenerator.cpp \
        utility/nullamplifier.cpp \
        utility/pauseelapsedtimer.cpp \
        utility/propamplifier.cpp \
        utility/sounddevice.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    MessageType.h \
    app.h \
    clientservice.h \
    clientsideparser.h \
    filter.h \
    flexibleproxymodel.h \
    ipluginupdateservice.h \
#    items/ImageSpLoader.h \
#    items/ImageSpNode.h \
    items/SpApplicationPrototype.h \
    items/customitem.h \
    items/discretegraphdrawer.h \
    items/mgkabstractmonitor.h \
    items/mgknumberviewprivate.h \
    items/monitors/mgkbronzerectmonitor.h \
    items/monitors/mgkcirclemonitor.h \
    items/painters/bronzecsnrpainter.h \
    items/painters/bronzesweeppainter.h \
    items/painters/bronzetucpainter.h \
    items/painters/csnrpainter.h \
    items/mgkrecorderprivate.h \
    items/monitors/mgkrectmonitorprivate.h \
    items/painters/doplerpainter.h \
    items/painters/epo160painter.h \
    items/painters/epo1painter.h \
    items/painters/monitorpainter.h \
    items/painters/sweeppainter.h \
    items/painters/tucpainter.h \
    mgkitembase.h \
    mgkitemsfactory.h \
    plugininterface.h \
    qzipreader_p.h \
    qzipwriter_p.h \
    simdatamanager.h \
    statecommunicator.h \
    updateservice.h \
    utility/amplifier.h \
    utility/delaynoisegenerator.h \
    utility/kdiscretizer.h \
    utility/kintegrator.h \
    utility/krangedistribution.h \
    utility/kvectordiscretizer.h \
    utility/noisegenerator.h \
    utility/nullamplifier.h \
    utility/pauseelapsedtimer.h \
    utility/propamplifier.h \
    utility/sounddevice.h

DEFINES += RESOURCES_PATH=\"\\\"$$PWD\\\"\"

message($$DEFINES)

CONFIG(release, debug|release){

    LIBS += -L$$PWD/../bin/libs/ -ldatabase
    LIBS += -L$$PWD/../bin/libs/ -lClientParser
    LIBS += -L$$PWD/../bin/libs/ -lTCPWorkers
    LIBS += -L$$PWD/../bin/libs/ -lAudioStreamingLibCore
    LIBS += -L$$PWD/../bin/libs/ -lklibcorelite
    LIBS += -L$$PWD/../bin/libs/ -lqnanopainter
    LIBS += -L$$PWD/../bin/libs/ -lSortFilterProxyModel


} else {
    win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/libs/database/debug/ -ldatabase
    else:unix: LIBS += -L$$OUT_PWD/../src/libs/database/ -ldatabase

    win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/libs/ClientParser/debug/ -lClientParser
    else:unix: LIBS += -L$$OUT_PWD/../src/libs/ClientParser/ -lClientParser

    win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/libs/TCPWorkers/debug/ -lTCPWorkers
    else:unix: LIBS += -L$$OUT_PWD/../src/libs/TCPWorkers/ -lTCPWorkers

    win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/libs/klibcorelite/debug/ -lklibcorelite
    else:unix: LIBS += -L$$OUT_PWD/../src/libs/klibcorelite/ -lklibcorelite

    win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/libs/AudioStreaming/AudioStreamingLib/AudioStreamingLibCore/debug/ -lAudioStreamingLibCore
    else:unix: LIBS += -L$$OUT_PWD/../src/libs/AudioStreaming/AudioStreamingLib/AudioStreamingLibCore/ -lAudioStreamingLibCore

win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/libs/qnanopainter/debug/ -lqnanopainter
else:unix: LIBS += -L$$OUT_PWD/../src/libs/qnanopainter/ -lqnanopainter

win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../src/libs/SortFilterProxyModel/debug/ -lSortFilterProxyModel
else:unix: LIBS += -L$$OUT_PWD/../src/libs/SortFilterProxyModel/ -lSortFilterProxyModel

}

INCLUDEPATH += $$PWD/../src/libs/database
DEPENDPATH += $$PWD/../src/libs/database

INCLUDEPATH += $$PWD/../src/libs/ClientParser
DEPENDPATH += $$PWD/../src/libs/ClientParser

INCLUDEPATH += $$PWD/../src/libs/TCPWorkers
DEPENDPATH += $$PWD/../src/libs/TCPWorkers

INCLUDEPATH += $$PWD/../src/libs/klibcorelite
DEPENDPATH += $$PWD/../src/libs/klibcorelite

INCLUDEPATH += $$PWD/../src/libs/qnanopainter
DEPENDPATH += $$PWD/../src/libs/qnanopainter

INCLUDEPATH += $$PWD/../src/libs/SortFilterProxyModel
DEPENDPATH += $$PWD/../src/libs/SortFilterProxyModel

INCLUDEPATH += $$PWD/../src/libs/AudioStreaming/AudioStreamingLib/AudioStreamingLibCore
DEPENDPATH += $$PWD/../src/libs/AudioStreaming/AudioStreamingLib/AudioStreamingLibCore

DISTFILES += \
    items/monitors/linefshader.fsh \
    items/monitors/linevshader.vsh

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libs/Audio/release/ -lAudio
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libs/Audio/debug/ -lAudio
#else:unix: LIBS += -L$$OUT_PWD/../libs/Audio/ -lAudio

#INCLUDEPATH += $$PWD/../libs/Audio
#DEPENDPATH += $$PWD/../libs/Audio
