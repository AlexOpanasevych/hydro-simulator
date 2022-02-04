#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = pluginsbase
QT -= gui
QT += quick
CONFIG += c++17
CONFIG += dll shared
DEFINES += QT_DEPRECATED_WARNINGS
CONFIG += shared dll
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DEPRECATED_WARNINGS

QML_IMPORT_NAME = pluginsbase
CONFIG(release, debug|release){
DESTDIR = $$PWD/../../../bin/libs/$$QML_IMPORT_NAME
}
#Inputs
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += PLUGINSBASE_LIBRARY
SOURCES += \
    base/global_messages/createpimsg.cpp \
    base/global_messages/removeitemmessage.cpp \
    base/global_messages/setmodeenginemsg.cpp \
    base/physical_SI.cpp \
    factories/controllerfactory.cpp \
    factories/sceneitemsfactory.cpp \
    externalInterface.cpp \
    tools/cmd.cpp \
    base/sceneitem.cpp \
    tools/trajectoryparser.cpp


HEADERS += \
    PluginsBase_global.h \
    base/global_messages/createpimsg.h \
    base/global_messages/removeitemmessage.h \
    base/global_messages/setmodeenginemsg.h \
    base/physical_SI.h \
    factories/controllerfactory.h \
    factories/sceneitemsfactory.h \
    tools/cmd.h \
    externalInterface.h \
    tools/iparser.h \
    base/sceneitem.h \
    tools/trajectoryparser.h


INCLUDEPATH += $$PWD/../../
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../bin/libs/core/ -lcore
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../bin/libs/core/ -lcore
else:unix: LIBS += -L$$PWD/../../../bin/libs/core/ -lcore
INCLUDEPATH += $$PWD/../../../bin/libs/core
DEPENDPATH += $$PWD/../../../bin/libs/core
win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../bin/libs/klibcorelite/ -lklibcorelite
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../bin/libs/klibcorelite/ -lklibcorelite
else:unix: LIBS += -L$$PWD/../../../bin/libs/klibcorelite/ -lklibcorelite
INCLUDEPATH += $$PWD/../../../bin/libs/klibcorelite
DEPENDPATH += $$PWD/../../../bin/libs/klibcorelite
CONFIG(release, debug|release){
#=============COPIES_STARTS=============#
qmldir.files = qmldir

default_copy.files = \ 
     externalInterface.h \ 
     PluginsBase_global.h
default_copy.path = $$DESTDIR/

base_copy.files = \ 
     base/sceneitem.h \ 
     base/physical_SI.h
base_copy.path = $$DESTDIR/base/

base_global_messages_copy.files = \ 
     base/global_messages/setmodeenginemsg.h \ 
     base/global_messages/removeitemmessage.h \ 
     base/global_messages/createpimsg.h
base_global_messages_copy.path = $$DESTDIR/base/global_messages/

factories_copy.files = \ 
     factories/sceneitemsfactory.h \ 
     factories/controllerfactory.h
factories_copy.path = $$DESTDIR/factories/

tools_copy.files = \ 
     tools/trajectoryparser.h \ 
     tools/iparser.h \ 
     tools/cmd.h
tools_copy.path = $$DESTDIR/tools/

COPIES += default_copy base_copy base_global_messages_copy factories_copy tools_copy
#=============COPIES_END=============#
}
