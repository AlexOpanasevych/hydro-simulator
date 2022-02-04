#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = AcousticCore
QT += qml quick concurrent
CONFIG += plugin c++17
CONFIG += qt
CONFIG += qmltypes

QML_IMPORT_NAME = AcousticCore
QML_IMPORT_MAJOR_VERSION = 1.0
DESTDIR = $$PWD/../../../bin/plugins/$$QML_IMPORT_NAME
QMLTYPES_FILENAME = $$DESTDIR/AcousticCore.qmltypes
uri = AcousticCore

#Inputs
SOURCES += \
    acousticcore_plugin.cpp \
    acousticsimulationcore.cpp \
    resources/gradientmaterial.cpp \
    resources/simplematerial.cpp \
    station/stationdata.cpp \
    station/stationitem.cpp \
    station/stationitemqsg.cpp \
    station/xnleaf.cpp \
    stationscore.cpp \
    wave/wavefielditem.cpp \
    wave/wavegenerator.cpp \
    wave/waveitem.cpp



HEADERS += \
    acousticcore_plugin.h \
    acousticsimulationcore.h \
    resources/gradientmaterial.h \
    resources/simplematerial.h \
    station/stationdata.h \
    station/stationitem.h \
    station/stationitemqsg.h \
    station/xnleaf.h \
    stationscore.h \
    wave/wavedata.h \
    wave/wavefielditem.h \
    wave/wavegenerator.h \
    wave/waveitem.h



DISTFILES = qmldir \
    qml/backgroundExtension/WaveCoreBackgroundExtension.qml \ 
    qml/settingsButton/WaveDebugSettingsButton.qml \ 
    qml/menuButton/StationMenuButton.qml \  
    qml/menuWindow/StationMenuWindow.qml \ 
    qml/settingsWindow/WaveDebugSettingsWindow.qml 

#=============QMLDIR_COPY=============#
!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$DESTDIR/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) "$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)" "$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

#====================SPECIAL_PROPERTIES_STARTS====================
#DEFINES += DEBUG

CONFIG(release, debug|release){
INCLUDEPATH += $$PWD/../../
LIBS += -L$$PWD/../../../bin/libs -lpluginsbase
LIBS += -L$$PWD/../../../bin/libs -lklibcorelite
LIBS += -L$$PWD/../../../bin/libs -lcore
}else{
win32: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/debug -lpluginsbase
else:unix: LIBS += -L$$OUT_PWD/../../libs/PluginsBase/ -lpluginsbase
win32: LIBS += -L$$OUT_PWD/../../libs/klibcorelite/debug/ -lklibcorelite
else:unix: LIBS += -L$$OUT_PWD/../../libs/klibcorelite/ -lklibcorelite
win32: LIBS += -L$$OUT_PWD/../../libs/core/debug -lcore
else:unix: LIBS += -L$$OUT_PWD/../../libs/core/ -lcore
}
INCLUDEPATH += $$PWD/../../libs/PluginsBase
DEPENDPATH += $$PWD/../../libs/PluginsBase
INCLUDEPATH += $$PWD/../../libs/klibcorelite
DEPENDPATH += $$PWD/../../libs/klibcorelite
INCLUDEPATH += $$PWD/../../libs/core
DEPENDPATH += $$PWD/../../libs/core
INCLUDEPATH += $$PWD/../../
QML_IMPORT_PATH += $$PWD/../../../bin/plugins/
#====================SPECIAL_PROPERTIES_ENDS====================

#=============COPIES_STARTS=============#
qmldir.files = qmldir

default_copy.files = \ 
     qmldir \
     depencies.json
default_copy.path = $$DESTDIR/

qml_backgroundExtension_copy.files = \ 
     qml/backgroundExtension/WaveCoreBackgroundExtension.qml
qml_backgroundExtension_copy.path = $$DESTDIR/qml/backgroundExtension/

qml_menuButton_copy.files = \ 
     qml/menuButton/StationMenuButton.qml
qml_menuButton_copy.path = $$DESTDIR/qml/menuButton/

qml_menuWindow_copy.files = \ 
     qml/menuWindow/StationMenuWindow.qml
qml_menuWindow_copy.path = $$DESTDIR/qml/menuWindow/

qml_settingsButton_copy.files = \ 
     qml/settingsButton/WaveDebugSettingsButton.qml
qml_settingsButton_copy.path = $$DESTDIR/qml/settingsButton/

qml_settingsWindow_copy.files = \ 
     qml/settingsWindow/WaveDebugSettingsWindow.qml
qml_settingsWindow_copy.path = $$DESTDIR/qml/settingsWindow/

COPIES += default_copy qml_backgroundExtension_copy qml_menuButton_copy qml_menuWindow_copy qml_settingsButton_copy qml_settingsWindow_copy
#=============COPIES_END=============#
