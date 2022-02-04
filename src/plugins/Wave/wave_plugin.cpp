#include "wave_plugin.h"

#include "waveitem.h"

#include <QDir>
#include <QPluginLoader>
#include <qqml.h>
#include <engine/messages/createitemmsg.h>
#include <tools/pluginloader.h>

void WavePlugin::setEngine(AbstractEngine *value)
{
    engine = value;
}

void WavePlugin::registerTypes(const char *uri)
{
    // @uri Wave
    qmlRegisterType<WaveItem>(uri, 1, 0, "WaveItem");
}

void WavePlugin::init(AbstractEngine *engine, QString appPath, const QVariantList &depencies)
{
    setTypeName("Wave");
    setEngine(engine);
    m_appPath = appPath;

    for (auto p : depencies){
        if (p.toString() == "AudioModule"){
            audioModulePlugin = dynamic_cast<QObject*>(pluginloader(p.toString(), m_appPath));
        }else{
            pluginloader(p.toString(), m_appPath);
        }
    }
}

ComponentsMap WavePlugin::getComponents()
{
#ifdef QT_NO_DEBUG
    return {
        {"tools",
            {
            {"Wave",
                {
                    {"settingsButton", m_appPath + "/plugins/" + typeName() + "/qml/" + typeName() + "SettingsButton.qml"},
                    {"settingsWindow", m_appPath + "/plugins/" + typeName() + "/qml/" + typeName() + "SettingsWindow.qml"}
                }
            }
        }
        }
    };
#else
    return {
        {"tools",
            {
            {"Wave",
                {
                    {"settingsButton", m_appPath + "/bin/plugins/" + typeName() + "/qml/" + typeName() + "DebugSettingsButton.qml"},
                    {"settingsWindow", m_appPath + "/bin/plugins/" + typeName() + "/qml/" + typeName() + "DebugSettingsWindow.qml"}
                }
            }
        }
        }
    };
#endif
}

