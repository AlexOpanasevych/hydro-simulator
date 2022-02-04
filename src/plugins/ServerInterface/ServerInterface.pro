#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = ServerInterface
QT += quick
QT += sql
QT += gui-private
QT += concurrent
QT += multimedia
QT += qml
QT += core
QT += widgets
CONFIG += c++17
CONFIG += qt
CONFIG += plugin
CONFIG += qmltypes
CONFIG += shared dll

QML_IMPORT_NAME = ServerInterface
QML_IMPORT_MAJOR_VERSION = 1.0
DESTDIR = $$PWD/../../../bin/plugins/$$QML_IMPORT_NAME
QMLTYPES_FILENAME = $$DESTDIR/ServerInterface.qmltypes
uri = ServerInterface



#Inputs
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_FATAL_WARNINGS
DEFINES += SERVERINTERFACE_LIBRARY
SOURCES += \
        src/abstractfactory.cpp \
        src/abstractsubsystem.cpp \
        src/app.cpp \
        src/client.cpp \
        src/serverinterface_plugin.cpp \
        src/stations/bronze.cpp \
        src/stations/bronzeSubsys/bronzereceiver.cpp \
        src/stations/bronzeSubsys/bronzetransmitter.cpp \
        src/stations/platina.cpp \
        src/stations/platinaSubsys/platinareceiver.cpp \
        src/stations/platinaSubsys/platinatransmitter.cpp \
        src/stations/utility/xnpressure.cpp \
        src/stations/utility/xnpressureconvertor.cpp \
        src/subsystems/mgkfsm.cpp \
        src/mode.cpp \
        src/subsystems/positionunit.cpp \
        src/subsystems/receiver.cpp \
        src/serverconfigurator.cpp \
        src/serverservice.cpp \
        src/serversideparser.cpp \
        src/state.cpp \
        src/station.cpp \
        src/subsystems/transmitter.cpp \
        src/updateservice.cpp \
        main.cpp


HEADERS += \
        serverinterface_global.h \
        serverinterface_plugin.h \
        src/MessageType.h \
        src/abstractfactory.h \
        src/abstractsubsystem.h \
        src/app.h \
        src/client.h \
        serverinterface_global.h \
        src/stations/bronze.h \
        src/stations/bronzeSubsys/bronzereceiver.h \
        src/stations/bronzeSubsys/bronzetransmitter.h \
        src/stations/platina.h \
        src/stations/platinaSubsys/platinareceiver.h \
        src/stations/platinaSubsys/platinatransmitter.h \
        src/stations/utility/xnpressure.h \
        src/stations/utility/xnpressureconvertor.h \
        src/subsystems/mgkfsm.h \
        src/mode.h \
        src/subsystems/positionunit.h \
        src/qzipreader_p.h \
        src/qzipwriter_p.h \
        src/subsystems/receiver.h \
        src/serverconfigurator.h \
        src/serverservice.h \
        src/serversideparser.h \
        src/state.h \
        src/station.h \
        src/subsystems/transmitter.h \
        src/updateservice.h \
        src/utility/SafeQueue.h

RESOURCES += qml.qrc

DISTFILES += \
        QML.db \
        qmldir \
        resources/qml/ClientsList.qml \
        resources/qml/ListDragItem.qml \
        resources/qml/PopupMessage.qml \
        resources/qml/ServerInterfaceSettingsWindow.qml \
        resources/qml/ServerInterfaceSettingsButton.qml \
        resources/qml/ServerMenuButton.qml \
        resources/qml/ServerMenuWindow.qml \
        resources/qml/Tiles.qml \
        src/subsystems/bronze/scripts/context.py \
        src/subsystems/bronze/scripts/logic.py \
        stateTransitions.json

QML_IMPORT_PATH += $$OUT_PWD/../../bin/plugins

CONFIG(release, debug|release){
INCLUDEPATH += $$PWD/../../
DEFINES += RELEASE
LIBS += -L$$PWD/../../../bin/libs/ -lcore
LIBS += -L$$PWD/../../../bin/libs/ -lklibcorelite
LIBS += -L$$PWD/../../../bin/libs/ -lpluginsbase
LIBS += -L$$PWD/../../../bin/libs/ -ldatabase
LIBS += -L$$PWD/../../../bin/libs/ -lClientParser
LIBS += -L$$PWD/../../../bin/libs/ -lTCPWorkers
LIBS += -L$$PWD/../../../bin/libs/ -lAudio

}else{
INCLUDEPATH += $$PWD/../../

win32: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/debug -lpluginsbase
else:unix: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/ -lpluginsbase

win32: LIBS += -L$$OUT_PWD/../../libs/core/debug -lcore
else:unix: LIBS += -L$$OUT_PWD/../../libs/core/ -lcore

win32: LIBS += -L$$OUT_PWD/../../libs/klibcorelite/debug/ -lklibcorelite
else:unix: LIBS += -L$$OUT_PWD/../../libs/klibcorelite/ -lklibcorelite

win32: LIBS += -L$$OUT_PWD/../../libs/database/debug/ -ldatabase
else:unix: LIBS += -L$$OUT_PWD/../../libs/database/ -ldatabase

win32: LIBS += -L$$OUT_PWD/../../libs/ClientParser/debug/ -lClientParser
else:unix: LIBS += -L$$OUT_PWD/../../libs/ClientParser/ -lClientParser

win32: LIBS += -L$$OUT_PWD/../../libs/TCPWorkers/debug/ -lTCPWorkers
else:unix: LIBS += -L$$OUT_PWD/../../libs/TCPWorkers/ -lTCPWorkers

win32: LIBS += -L$$OUT_PWD/../../libs/Audio/debug/ -lAudio
else:unix: LIBS += -L$$OUT_PWD/../../libs/Audio/ -lAudio
}
INCLUDEPATH += $$PWD/../../libs/PluginsBase
DEPENDPATH += $$PWD/../../libs/PluginsBase

INCLUDEPATH += $$PWD/../../libs/Audio
DEPENDPATH += $$PWD/../../libs/Audio

INCLUDEPATH += $$PWD/../../libs/TCPWorkers
DEPENDPATH += $$PWD/../../libs/TCPWorkers

INCLUDEPATH += $$PWD/../../libs/ClientParser
DEPENDPATH += $$PWD/../../libs/ClientParser

INCLUDEPATH += $$PWD/../../libs/database
DEPENDPATH += $$PWD/../../libs/database

INCLUDEPATH += $$PWD/../../libs/klibcorelite
DEPENDPATH += $$PWD/../../libs/klibcorelite

INCLUDEPATH += $$PWD/../../libs/core
DEPENDPATH += $$PWD/../../libs/core

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
     QML.db \
     stateTransitions.json \
     depencies.json

default_copy.path = $$DESTDIR/

resources_qml_copy.files = \
     resources/qml/Tiles.qml \
     resources/qml/PopupMessage.qml \
     resources/qml/ListDragItem.qml \
     resources/qml/ClientsList.qml \
     resources/qml/ServerMenuButton.qml \
     resources/qml/ServerMenuWindow.qml \
     resources/qml/ServerInterfaceSettingsButton.qml \
     resources/qml/ServerInterfaceSettingsWindow.qml
resources_qml_copy.path = $$DESTDIR/resources/qml/

src_subsystems_bronze_scripts_copy.files = \
    src/subsystems/bronze/scripts/context.py \
    src/subsystems/bronze/scripts/logic.py
src_subsystems_bronze_scripts_copy.path = $$DESTDIR/src/subsystems/bronze/scripts/

app_copy.files = \
    src/subsystems/bronze/scripts/context.py \
    src/subsystems/bronze/scripts/logic.py
src_subsystems_bronze_scripts_copy.path = $$PWD/../../../build/src/app/

include(../../libs/pythonqt/build/common_libs.prf)
include(../../libs/pythonqt/build/PythonQt_libs.prf)
include(../../libs/pythonqt/build/PythonQt_QtAll_libs.prf)

COPIES += default_copy resources_qml_copy src_subsystems_bronze_scripts_copy
#=============COPIES_END=============#
