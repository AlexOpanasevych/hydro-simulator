#ifndef ENGINE_H
#define ENGINE_H

#include <QFuture>
#include <libs/core/abstractengine.h>
#include <libs/PluginsBase/base/global_messages/createpimsg.h>
#include <libs/PluginsBase/base/global_messages/setmodeenginemsg.h>
#include <libs/PluginsBase/base/global_messages/removeitemmessage.h>
#include <libs/PluginsBase/engine/messages/setshippathmsg.h>

class QTimer;
class ControllerFactory;
class Engine : public QObject, public AbstractEngine
{
public:
    Engine(QObject *parent = nullptr);
    ~Engine();

    // AbstractEngine interface
public:
    void start();
    void stop();

    void setControllers(const QHash<QString, ControllerFactory *> &value);
    void setCreatebleTypes(const QStringList &value);

protected:
    int lastCreatedPIID = 0;

private:
    QFuture<void> future;
    QTimer* m_timer;
    CreatePIMsgAns proceedCreateItemMsg(CreatePIMsg msg);
    RemoveItemMessageAns proceedRemoveItemMsg(RemoveItemMessage msg);
    SetModeEngineMsgAns proceedSetEngineModeMsg(SetModeEngineMsg msg);

    QStringList createbleTypes;
    QHash<QString, ControllerFactory*> controllers;

    bool working = true;

    static QMutex m;

    // AbstractEngine interface
};

#endif // ENGINE_H
