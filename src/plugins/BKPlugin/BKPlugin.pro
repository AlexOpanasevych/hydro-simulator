TEMPLATE = lib subdirs
TARGET = BKPlugin
QT += qml quick widgets serialport concurrent
CONFIG += plugin c++17
CONFIG += qt
CONFIG += qmltypes

QML_IMPORT_NAME = BKPlugin
QML_IMPORT_MAJOR_VERSION = 1.0
DESTDIR = $$PWD/../../../bin/plugins/$$QML_IMPORT_NAME
QMLTYPES_FILENAME = $$DESTDIR/BKPlugin.qmltypes
uri = BKPlugin

# Input
SOURCES += \
        bkplugin_plugin.cpp \
        bk.cpp \
        gsnworker.cpp \
        motordriverworker.cpp \
        nov_helper/nov_packet_parser.cpp \
        novworker.cpp

HEADERS += \
        bkplugin_plugin.h \
        bk.h \
        gsnworker.h \
        motordriverworker.h \
        nov_helper/nov_packet_parser.h \
        novworker.h

DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$DESTDIR/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) "$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)" "$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

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

qmldir.files = qmldir

default_copy.files = \
     qmldir
default_copy.path = $$DESTDIR/
#unix {
#    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \., /)
#    qmldir.path = $$installPath
#    target.path = $$installPath
#    INSTALLS += target qmldir
#}

qml_settingsButton_copy.files = \
     qml/settingsButton/BKSettingsButton.qml
qml_settingsButton_copy.path = $$DESTDIR/qml/settingsButton/

COPIES += default_copy qml_settingsButton_copy

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/kbus/release/ -lKBus
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/kbus/debug/ -lKBus
else:unix: LIBS += -L$$OUT_PWD/../../libs/kbus/ -lKBus

INCLUDEPATH += $$PWD/../../libs/kbus
DEPENDPATH += $$PWD/../../libs/kbus

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../../libs/vdebug/release/ -lvdebug
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../../libs/vdebug/debug/ -lvdebug
else:unix: LIBS += -L$$OUT_PWD/../../libs/vdebug/ -lvdebug

INCLUDEPATH += $$PWD/../../libs/vdebug
DEPENDPATH += $$PWD/../../libs/vdebug
