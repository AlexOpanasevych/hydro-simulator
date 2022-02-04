#include "acousticcore_plugin.h"

#include <QDir>
#include <qqml.h>
#include <cdebug.h>
#include <QPluginLoader>
#include <tools/pluginloader.h>
#include <wave/wavefielditem.h>
#include <station/stationdata.h>
#include <acousticsimulationcore.h>
#include <station/stationitemqsg.h>
#include <engine/messages/createitemmsg.h>

void AcousticCorePlugin::registerTypes(const char *uri)
{
    // @uri AcousticCore
    qmlRegisterSingletonType<AcousticSimulationCore>(uri, 1, 0, "AcousticSimulationCore", &AcousticSimulationCore::qmlInstance);
    qmlRegisterType<WaveFieldItem>(uri, 1, 0, "WaveFieldItem");
    qmlRegisterType<StationData>(uri, 1, 0, "StationData");
    qmlRegisterType<StationItemQSG>(uri, 1, 0, "StationItemQSG");
}

void AcousticCorePlugin::init(AbstractEngine *engine, QString appPath, const QVariantList &depencies)
{
    setTypeName("AcousticCore");
    m_engine = engine;
    m_appPath = appPath;

    for (auto p : depencies){
        if (p.toString() == "AudioModule"){
            audioModulePlugin = dynamic_cast<QObject*>(pluginloader(p.toString(), m_appPath));
        }else{
            pluginloader(p.toString(), m_appPath);
        }
    }
}

ComponentsMap AcousticCorePlugin::getComponents()
{
#ifdef QT_NO_DEBUG
    return {
        {"tools",
            {
                {"AcousticCore",
                    {
                        {   "backgroundExtension", m_appPath + "/plugins/" + typeName() + "/qml/backgroundExtension/WaveCoreBackgroundExtension.qml" },
                        {   "menuButton",          m_appPath + "/plugins/" + typeName() + "/qml/menuButton/StationMenuButton.qml" },
                        {   "menuWindow",          m_appPath + "/plugins/" + typeName() + "/qml/menuWindow/StationMenuWindow.qml" }
                    }
                }
            }
        }
    };

#else
    return {
        {"tools",
            {
                {"AcousticCore",
                    {
                        {   "backgroundExtension", m_appPath + "/bin/plugins/" + typeName() + "/qml/backgroundExtension/WaveCoreBackgroundExtension.qml" },
                        {   "menuButton",          m_appPath + "/bin/plugins/" + typeName() + "/qml/menuButton/StationMenuButton.qml" },
                        {   "menuWindow",          m_appPath + "/bin/plugins/" + typeName() + "/qml/menuWindow/StationMenuWindow.qml" }
                    }
                }
            }
        }
    };
#endif
}

