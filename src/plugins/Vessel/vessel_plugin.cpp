#include "vessel_plugin.h"
#include <QQmlComponent>
#include <qqml.h>

#include <base/global_messages/setmodeenginemsg.h>
#include <engine/controllers/vesselcontroller.h>
#include <engine/messages/changecoordsmsg.h>
#include <engine/messages/itemstartmsg.h>
#include <sceneItems/vessel.h>
#include <cdebug.h>
#include <QDir>
#include <QPluginLoader>
#include <libs/PluginsBase/tools/pluginloader.h>

void VesselPlugin::registerTypes(const char *uri)
{
    // @uri Vessel
    qmlRegisterType<Vessel>(uri, 1, 0, "Vessel");
}

void VesselPlugin::init(AbstractEngine *engine, QString appPath, const QVariantList &depencies)
{
    setTypeName("Vessel");
    m_appPath = appPath;
    configPath = m_appPath + configPath;
    this->engine = engine;

    for (auto p : depencies){
        if (p.toString() == "AudioModule"){
            audioModulePlugin = dynamic_cast<QObject*>(pluginloader(p.toString(), m_appPath));
        }else{
            pluginloader(p.toString(), m_appPath);
        }
    }

    if (audioModulePlugin != nullptr){
        registerPluginEngineCalback<ItemStartMsg>(engine, [this](ItemStartMsg ans){
#ifdef QT_NO_DEBUG
            QVariantMap data = {{"name", QDir(m_appPath + "/plugins/AudioModule/sounds").absolutePath() + "/shipSound.sd"},
                                {"interval", 0},
                               };
#else
            QVariantMap data = {{"name", QDir(m_appPath + "/bin/plugins/AudioModule/sounds").absolutePath() + "/shipSound.sd"},
                                {"interval", 0},
                               };
#endif
            initSound("raw", ans.id, data);
        });

        registerPluginEngineCalback<ItemStopMsg>(engine, [this](ItemStopMsg msg){
            stopSound(msg.id);
        });

        registerPluginEngineCalback<SetModeEngineMsgAns>(engine, [this](SetModeEngineMsgAns msg){
            qDebug() << "stop all sounds" << msg.mode;
            stopSound();
        });

    }

    createItemFactory<Vessel>();
    createControllerFactory<VesselController>();
}

ComponentsMap VesselPlugin::getComponents()
{
#ifdef QT_NO_DEBUG
    return {
        {"scene", {
                {"Vessel",
                    {
                        {"item" ,             m_appPath + "/plugins/"+ typeName() + "/qml/items/" +       typeName() + "Item.qml"},
                        {"menuButton" ,       m_appPath + "/plugins/"+ typeName() + "/qml/menuButton/" +  typeName() + "MenuButton.qml"},
                        {"menuWindow" ,       m_appPath + "/plugins/"+ typeName() + "/qml/menuWindow/" +  typeName() + "MenuWindow.qml"},
                        {"properties" ,       m_appPath + "/plugins/"+ typeName() + "/qml/properties/" +  typeName() + "Properties.qml"},
                        {"sceneLoader" ,      m_appPath + "/plugins/"+ typeName() + "/qml/sceneLoader/" + typeName() + "SceneLoader.qml"     },
                        {"addItemMenuButton", m_appPath + "/plugins/"+ typeName() + "/qml/menuButton/" +  typeName() + "AddItemMenuButton.qml"},
                        {"addItemMenuWindow", m_appPath + "/plugins/"+ typeName() + "/qml/menuWindow/" +  typeName() + "AddItemMenuWindow.qml"}
                    }
                }
            }
        }
    };
#else
    return {
        {"scene", {
                {"Vessel",
                    {
                        {"item" ,             m_appPath + "/bin/plugins/"+ typeName() + "/qml/items/" +       typeName() + "Item.qml"},
                        {"menuButton" ,       m_appPath + "/bin/plugins/"+ typeName() + "/qml/menuButton/" +  typeName() + "MenuButton.qml"},
                        {"menuWindow" ,       m_appPath + "/bin/plugins/"+ typeName() + "/qml/menuWindow/" +  typeName() + "MenuWindow.qml"},
                        {"properties" ,       m_appPath + "/bin/plugins/"+ typeName() + "/qml/properties/" +  typeName() + "Properties.qml"},
                        {"sceneLoader" ,      m_appPath + "/bin/plugins/"+ typeName() + "/qml/sceneLoader/" + typeName() + "SceneLoader.qml"     },
                        {"addItemMenuButton", m_appPath + "/bin/plugins/"+ typeName() + "/qml/menuButton/" +  typeName() + "AddItemMenuButton.qml"},
                        {"addItemMenuWindow", m_appPath + "/bin/plugins/"+ typeName() + "/qml/menuWindow/" +  typeName() + "AddItemMenuWindow.qml"}
                    }
                }
            }
        }
    };
#endif
}

QStringList VesselPlugin::creatableTypes()
{
    return {"Vessel"};
}

void VesselPlugin::initSound(QString soundName, int uniqueId, QVariantMap data)
{
    QMetaObject::invokeMethod(audioModulePlugin, "addSound", Qt::DirectConnection, Q_ARG(int, uniqueId), Q_ARG(QString, soundName), Q_ARG(QVariantMap, data));
}

void VesselPlugin::stopSound(int id, QString name)
{
    QMetaObject::invokeMethod(audioModulePlugin, "stopSound", Qt::DirectConnection, Q_ARG(int, id), Q_ARG(QString, name));
}
