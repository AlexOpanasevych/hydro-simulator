#include "tools_plugin.h"

#include "cusommonitor.h"
#include "myitem.h"

#include <QDir>
#include <QPluginLoader>
#include <qqml.h>
#include <tools/pluginloader.h>

void ToolsPlugin::registerTypes(const char *uri)
{
    // @uri Tools
    qmlRegisterType<MyItem>(uri, 1, 0, "MyItem");
    qmlRegisterType<CustomMonitor>(uri, 1, 0, "CustomMonitor");
}

void ToolsPlugin::init(AbstractEngine *engine, QString appPth, const QVariantList &depencies)
{
    Q_UNUSED(engine)
    m_appPath = appPth;
    setTypeName("Tools");

    for (auto p : depencies){
        pluginloader(p.toString(), m_appPath);
    }
}

ComponentsMap ToolsPlugin::getComponents()
{
    return {{"tools",{
                {"Tools",
                    {
                        {"settingsWindow",      m_appPath + "/bin/plugins/" + typeName() + "/qml/Test/TestSettingsWindow.qml"},
                        {"settingsButton",      m_appPath + "/bin/plugins/" + typeName() + "/qml/Test/TestSettingsButton.qml"},
                        {"backgroundExtension", m_appPath + "/bin/plugins/" + typeName() + "/qml/Test/TestBackgroundExtension.qml"}
                    }
                }
            }}};
}

ToolsPlugin::ToolsPlugin()
{

}

ToolsPlugin::~ToolsPlugin()
{

}

//QList<QString> ToolsPlugin::sceneItemList()
//{
//    return {"distanceTool"};
//}
//
//QList<SceneItemsFactory *> ToolsPlugin::getCreators()
//{
//    return creators;
//}
//
//void ToolsPlugin::initCreators()
//{
//
//}
//
//void ToolsPlugin::setEngine(QQmlEngine *engine)
//{
//    this->engine = engine;
//}
//
//QList<ControllerFactory *> ToolsPlugin::getControllers()
//{
//    return {};
//}
//
//QList<SampleDataFactory *> ToolsPlugin::getSampleDatas()
//{
//    return {};
//}
//
//QList<ControllerSampleDataFactory *> ToolsPlugin::getSampleDataControllers()
//{
//    return {};
//}
