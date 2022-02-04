QT += serialport
QT += concurrent
QT += widgets

TEMPLATE = lib
DEFINES += KBUS_LIBRARY

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    base/kbusworker.cpp \
    buses/kserialbusthreaded.cpp \
    coms/KRawParser.cpp \
    gui/kbusconfigurationmenu.cpp \
    gui/kbusconfigurationwidget.cpp \
    gui/kbusconfigurationwindow.cpp \
    kbusfactory.cpp \
    tools/circlebuf.cpp \
    tools/convert.cpp

HEADERS += \
    KBus_global.h \
    base/KBus.h \
    base/KCommunicator.h \
    base/kbusworker.h \
    buses/kserialbusthreaded.h \
    coms/KRawParser.h \
    gui/busparams.h \
    gui/kbusconfigurationmenu.h \
    gui/kbusconfigurationwidget.h \
    gui/kbusconfigurationwindow.h \
    kbusfactory.h \
    tools/circlebuf.h \
    tools/convert.h

# Default rules for deployment.
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../vdebug/release/ -lvdebug
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../vdebug/debug/ -lvdebug
else:unix: LIBS += -L$$OUT_PWD/../vdebug/ -lvdebug


INCLUDEPATH += $$PWD/../VDebug
DEPENDPATH += $$PWD/../VDebug

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../PluginsBase/release/ -lpluginsbase
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../PluginsBase/debug/ -lpluginsbase
else:unix: LIBS += -L$$OUT_PWD/../PluginsBase/ -lpluginsbase

INCLUDEPATH += $$PWD/../PluginsBase
DEPENDPATH += $$PWD/../PluginsBase
