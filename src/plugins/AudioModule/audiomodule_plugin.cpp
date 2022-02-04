#include "audiomodule_plugin.h"

#include "audiomodule.h"
#include <QDir>
#include <QPluginLoader>
#include <qqml.h>
#include <tools/pluginloader.h>

ComponentsMap AudioModulePlugin::getComponents()
{
    assert(!typeName().isEmpty());
#ifdef QT_NO_DEBUG
    return {{"tools", {

            {"AudioModule",
                {
                    {"settingsButton", m_appPath + "/plugins/" + typeName() + "/qml/" + typeName() + "SettingsButton.qml"},
                    {"settingsWindow", m_appPath + "/plugins/" + typeName() + "/qml/" + typeName() + "SettingsWindow.qml"}
                }
            }
        }}};
#else
    return {{"tools", {

            {"AudioModule",
                {
                    {"settingsButton", m_appPath + "/bin/plugins/" + typeName() + "/qml/" + typeName() + "SettingsButton.qml"},
                    {"settingsWindow", m_appPath + "/bin/plugins/" + typeName() + "/qml/" + typeName() + "SettingsWindow.qml"}
                }
            }
        }}};
#endif
}

void AudioModulePlugin::init(AbstractEngine *engine, QString appPath, const QVariantList &depencies)
{
    setTypeName("AudioModule");
    m_appPath = appPath;
    this->engine = engine;

    for (auto p : depencies){
        if (p.toString() == "AudioModule"){
            m_serverInterface = dynamic_cast<AbstractServerInterface*>(pluginloader(p.toString(), m_appPath));
        }else{
            pluginloader(p.toString(), m_appPath);
        }
    }
//    AudioModule::instance()->audioService()->addSound("sin", {{"frequency", 2000}});
}

void AudioModulePlugin::registerTypes(const char *uri)
{
    // @uri AudioModule
    qmlRegisterType<AudioModule>(uri, 1, 0, "AudioModule");
}

void AudioModulePlugin::addSound(int id, QString soundName, QVariantMap data)
{
    AudioModule::instance()->audioService()->addSound(id, soundName, data);
}

void AudioModulePlugin::stopSound(int id, QString soundName)
{
    AudioModule::instance()->audioService()->stopSound(id, soundName);
}
