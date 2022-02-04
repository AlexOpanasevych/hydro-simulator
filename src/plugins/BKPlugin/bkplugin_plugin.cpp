#include "bkplugin_plugin.h"

#include "bk.h"

#include <qqml.h>
#include <tools/pluginloader.h>

void BKPluginPlugin::registerTypes(const char *uri)
{
    qmlRegisterType<BK>(uri, 1, 0, "BK");
}

void BKPluginPlugin::init(AbstractEngine *engine, QString appPath, const QVariantList &depencies)
{
    Q_UNUSED(depencies)
    setTypeName("BKPlugin");
    m_engine = engine;
    m_appPath = appPath;
    for (auto p : depencies){
        pluginloader(p.toString(), m_appPath);
    }

//    bk = new BK();
    BK::instance()->setEngine(m_engine);
}

ComponentsMap BKPluginPlugin::getComponents()
{
#ifdef QT_NO_DEBUG
    return {
        {"tools",
            {
                {"BKPlugin",
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
                {"BKPlugin",
                    {
//                        {   "backgroundExtension", m_appPath + "/bin/plugins/" + typeName() + "/qml/backgroundExtension/WaveCoreBackgroundExtension.qml" },
                        {   "settingsButton",          m_appPath + "/bin/plugins/" + typeName() + "/qml/settingsButton/BKSettingsButton.qml" },
//                        {   "menuWindow",          m_appPath + "/bin/plugins/" + typeName() + "/qml/menuWindow/StationMenuWindow.qml" }
                    }
                }
            }
        }
    };
#endif
}

