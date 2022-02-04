#=============EDITED_BY_PLUGINHELPER=============#
TEMPLATE = lib
TARGET = pluginsbase
QT -= gui
QT += quick
CONFIG += c++17
CONFIG += shared dll

CONFIG(release, debug|release){
DESTDIR = $$PWD/../../../bin/libs/
}
#Inputs
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += PLUGINSBASE_LIBRARY
SOURCES += \
        AbstractServerInterface.cpp \
        base/global_messages/createpimsg.cpp \
        base/global_messages/removeitemmessage.cpp \
        base/global_messages/setmodeenginemsg.cpp \
        base/physical_SI.cpp \
        base/scenebase.cpp \
        base/sceneprivateaccessor.cpp \
        engine/messages/addsegmentmessage.cpp \
        engine/messages/changeanglemsg.cpp \
        engine/messages/changecoordsmsg.cpp \
        engine/messages/changevelocitymsg.cpp \
        engine/messages/createitemmsg.cpp \
        engine/messages/itemstartmsg.cpp \
        engine/messages/setshippathmsg.cpp \
        engine/messages/settorpedoparams.cpp \
        engine/messages/wavemessage.cpp \
        factories/controllerfactory.cpp \
        factories/sceneitemsfactory.cpp \
        iphysicalitem.cpp \
        isceneiteminterface.cpp \
        tools/cmd.cpp \
        base/sceneitem.cpp \
        AbstractPluginInterface.cpp \
        tools/globalconfig.cpp \
        tools/trajectoryparser.cpp

HEADERS += \
        AbstractServerInterface.h \
        base/global_messages/createpimsg.h \
        base/global_messages/removeitemmessage.h \
        base/global_messages/setmodeenginemsg.h \
        base/physical_SI.h \
        base/scenebase.h \
        base/sceneprivateaccessor.h \
        cdebug.h \
        engine/messages/addsegmentmessage.h \
        engine/messages/changeanglemsg.h \
        engine/messages/changecoordsmsg.h \
        engine/messages/changevelocitymsg.h \
        engine/messages/createitemmsg.h \
        engine/messages/itemstartmsg.h \
        engine/messages/setshippathmsg.h \
        engine/messages/settorpedoparams.h \
        engine/messages/wavemessage.h \
        factories/controllerfactory.h \
        factories/sceneitemsfactory.h \
        iphysicalitem.h \
        isceneiteminterface.h \
        pluginsbase_global.h \
        tools/cmd.h \
        tools/globalconfig.h \
        tools/iparser.h \
        base/sceneitem.h \
        tools/pluginloader.h \
        tools/trajectoryparser.h \
        AbstractPluginInterface.h

#================SPECIAL_PROPERTIES_STARTS================
CONFIG(release, debug|release){
INCLUDEPATH += $$PWD/../../
DEFINES += QT_NO_DEBUG_OUTPUT
LIBS += -L$$PWD/../../../bin/libs/ -lcore
LIBS += -L$$PWD/../../../bin/libs/ -lklibcorelite
}else{
INCLUDEPATH += $$PWD/../../

win32: LIBS += -L$$OUT_PWD/../core/debug -lcore
else:unix: LIBS += -L$$OUT_PWD/../core/ -lcore

win32: LIBS += -L$$OUT_PWD/../klibcorelite/debug -lklibcorelite
else:unix: LIBS += -L$$OUT_PWD/../klibcorelite/ -lklibcorelite
}

INCLUDEPATH += $$PWD/../klibcorelite
DEPENDPATH += $$PWD/../klibcorelite

INCLUDEPATH += $$PWD/../core
DEPENDPATH += $$PWD/../core
#================SPECIAL_PROPERTIES_ENDS================
