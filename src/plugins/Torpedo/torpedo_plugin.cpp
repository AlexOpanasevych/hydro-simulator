#include "torpedo_plugin.h"
#include <QQmlComponent>
#include <qqml.h>
#include <QDir>
#include <QPluginLoader>

#include <sceneItems/torpedo.h>
#include <engine/controllers/torpedocontroller.h>
#include <engine/messages/itemstartmsg.h>
#include <tools/pluginloader.h>

void TorpedoPlugin::registerTypes(const char *uri)
{
    // @uri Torpedo
    qmlRegisterType<Torpedo>(uri, 1, 0, "Torpedo");
}

void TorpedoPlugin::init(AbstractEngine *engine, QString appPath, const QVariantList &depencies)
{
    setTypeName("Torpedo");
    m_appPath = appPath;
    configPath = appPath + configPath;
    this->engine = engine;
    for (auto p : depencies){
        if (p.toString() == "AudioModule"){
            audioModulePlugin = dynamic_cast<QObject*>(pluginloader(p.toString(), m_appPath));
        }else{
            pluginloader(p.toString(), m_appPath);
        }
    }

    if (audioModulePlugin != nullptr){
        registerPluginEngineCalback<ChangeCoordsMsgAns>(engine, [this](ChangeCoordsMsgAns ans){
            if(ans.success) {
                QVariantMap data = {{"frequency", 2000},
                                    {"interval", 100},
                                   };
                initSound("sin", ans.id, data);

            }
        });
    }
    //    initSound(data);


    //    registerPluginEngineCalback<ItemStartMsg>(engine, [this](ItemStartMsg ans){
    //        qDebug() << "changed coords";
    //        QVariantMap data = {{"frequency", 2000},
    //                            {"interval", 100},
    //                           };
    //        initSound("sin", ans.id, data);
    //    });

    //    registerPluginEngineCalback<ItemStopMsg>(engine, [this](ItemStopMsg msg){
    //        stopSound(msg.id);
    //    });


    createItemFactory<Torpedo>();
    createControllerFactory<TorpedoController>();
}

ComponentsMap TorpedoPlugin::getComponents()
{
    assert(!typeName().isEmpty());
#ifdef QT_NO_DEBUG
    return {
        {"scene", {
                {"Torpedo",
                    {
                        {"item" ,             m_appPath + "/plugins/" + typeName() + "/qml/items/" +       typeName() + "Item.qml"},
                        {"menuButton" ,       m_appPath + "/plugins/" + typeName() + "/qml/menuButton/" +  typeName() + "MenuButton.qml"},
                        {"menuWindow" ,       m_appPath + "/plugins/" + typeName() + "/qml/menuWindow/" +  typeName() + "MenuWindow.qml"},
                        {"properties" ,       m_appPath + "/plugins/" + typeName() + "/qml/properties/" +  typeName() + "Properties.qml"},
                        {"sceneLoader" ,      m_appPath + "/plugins/" + typeName() + "/qml/sceneLoader/" + typeName() + "SceneLoader.qml"     },
                        {"addItemMenuButton", m_appPath + "/plugins/" + typeName() + "/qml/menuButton/" +  typeName() + "AddItemMenuButton.qml"},
                        {"addItemMenuWindow", m_appPath + "/plugins/" + typeName() + "/qml/menuWindow/" +  typeName() + "AddItemMenuWindow.qml"}
                    }
                }
            }
        }
    };
#else
    return {
        {"scene", {
                {"Torpedo",
                    {
                        {"item" ,             m_appPath + "/bin/plugins/" + typeName() + "/qml/items/" +       typeName() + "Item.qml"},
                        {"menuButton" ,       m_appPath + "/bin/plugins/" + typeName() + "/qml/menuButton/" +  typeName() + "MenuButton.qml"},
                        {"menuWindow" ,       m_appPath + "/bin/plugins/" + typeName() + "/qml/menuWindow/" +  typeName() + "MenuWindow.qml"},
                        {"properties" ,       m_appPath + "/bin/plugins/" + typeName() + "/qml/properties/" +  typeName() + "Properties.qml"},
                        {"sceneLoader" ,      m_appPath + "/bin/plugins/" + typeName() + "/qml/sceneLoader/" + typeName() + "SceneLoader.qml"     },
                        {"addItemMenuButton", m_appPath + "/bin/plugins/" + typeName() + "/qml/menuButton/" +  typeName() + "AddItemMenuButton.qml"},
                        {"addItemMenuWindow", m_appPath + "/bin/plugins/" + typeName() + "/qml/menuWindow/" +  typeName() + "AddItemMenuWindow.qml"}
                    }
                }
            }
        }
    };
#endif
}

QStringList TorpedoPlugin::creatableTypes()
{
    return {"Torpedo"};
}

void TorpedoPlugin::initSound(QString soundName, int uniqueId, QVariantMap data)
{
    QMetaObject::invokeMethod(audioModulePlugin, "addSound", Qt::DirectConnection, Q_ARG(int, uniqueId), Q_ARG(QString, soundName), Q_ARG(QVariantMap, data));
}

void TorpedoPlugin::stopSound(int id, QString name)
{
    QMetaObject::invokeMethod(audioModulePlugin, "stopSound", Qt::DirectConnection, Q_ARG(int, id), Q_ARG(QString, name));
}
