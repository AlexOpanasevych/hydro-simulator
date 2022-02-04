#include "saveprovider_plugin.h"

#include <QDir>
#include <QPluginLoader>
#include <qqml.h>
#include <savedata.h>
#include <savemanager.h>
#include <base/scenebase.h>
#include <base/sceneprivateaccessor.h>
#include <cdebug.h>
#include <tools/pluginloader.h>

void SaveProviderPlugin::registerTypes(const char *uri)
{
    // @uri SaveProvider
    qmlRegisterType<SaveManager>(uri, 1, 0, "SaveManager");
    qmlRegisterType<SaveData>(uri, 1, 0, "SaveData");
}

void SaveProviderPlugin::init(AbstractEngine *engine, QString appPath, const QVariantList &depencies)
{
    Q_UNUSED(engine)
    setTypeName("SaveProvider");
    m_appPath = appPath;

    for (auto p : depencies){
        pluginloader(p.toString(), m_appPath);
    }
}

ComponentsMap SaveProviderPlugin::getComponents()
{
#ifdef QT_NO_DEBUG
    return {{"tools",{
                {"SaveProvider",
                    {
                       {"menuWindow", m_appPath + "/plugins/SaveProvider/qml/menuWindow/SaveMenuWindow.qml"},
                       {"menuButton", m_appPath + "/plugins/SaveProvider/qml/menuButton/SaveMenuButton.qml"}
                    }
                }
            }}};
#else
    return {{"tools",{
                {"SaveProvider",
                    {
                       {"menuWindow", m_appPath + "/bin/plugins/SaveProvider/qml/menuWindow/SaveMenuWindow.qml"},
                       {"menuButton", m_appPath + "/bin/plugins/SaveProvider/qml/menuButton/SaveMenuButton.qml"}
                    }
                }
            }}};
#endif
}

