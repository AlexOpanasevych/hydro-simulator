#include "serverinterface_plugin.h"
#include "src/app.h"

#include <abstractengine.h>
#include "serverservice.h"
#include <streamconnector.h>
#include <tools/pluginloader.h>


void ServerInterfacePlugin::registerTypes(const char *uri)
{
    qmlRegisterSingletonType(QUrl("file:///resources/qml/DragHelper.qml"), "DragHelper", 1, 0, "DragHelper");
    Q_UNUSED(uri)
}

void ServerInterfacePlugin::init(AbstractEngine * eng, QString appPath, const QVariantList &depencies)
{
    setTypeName("ServerInterface");
    this->m_engine = eng;

    for (auto p : depencies){
        pluginloader(p.toString(), m_appPath);
    }

    m_appPath = appPath;
    App::instance()->initConfigurator(appPath);
    App::instance()->server()->platinaStation()->setEngine(eng);
    App::instance()->server()->bronzeStation()->setEngine(eng);
    App::instance()->server()->bronzeStation()->setAppPath(appPath);

    registerPluginEngineCalback<CreatePIMsgAns>(engine(), std::bind(&ServerInterfacePlugin::testMsgHandler, this, std::placeholders::_1));
}

ComponentsMap ServerInterfacePlugin::getComponents()
{
#ifdef QT_NO_DEBUG
    return {{"tools", {
                {"ServerInterface",
                    {
                        {"settingsButton", m_appPath + "/plugins/" + typeName() + "/resources/qml/" + typeName() + "SettingsButton.qml"},
                        {"settingsWindow", m_appPath + "/plugins/" + typeName() + "/resources/qml/" + typeName() + "SettingsWindow.qml"},
                        {"menuWindow", m_appPath + "/plugins/" + typeName() + "/resources/qml/menuWindow/ServerMenuWindow.qml"},
                        {"menuButton", m_appPath + "/plugins/" + typeName() + "/resources/qml/menuButton/ServerMenuButton.qml"}
                    }
                }
            }}};
#else
    return {{"tools", {
                {"ServerInterface",
                    {
                        {"settingsButton", m_appPath + "/bin/plugins/" + typeName() + "/resources/qml/" + typeName() + "SettingsButton.qml"},
                        {"settingsWindow", m_appPath + "/bin/plugins/" + typeName() + "/resources/qml/" + typeName() + "SettingsWindow.qml"},
                        {"menuWindow", m_appPath + "/bin/plugins/" + typeName() + "/resources/qml/ServerMenuWindow.qml"},
                        {"menuButton", m_appPath + "/bin/plugins/" + typeName() + "/resources/qml/ServerMenuButton.qml"}
                    }
                }
            }}};
#endif
}

AbstractEngine *ServerInterfacePlugin::engine() const
{
    return m_engine;
}

void ServerInterfacePlugin::registerStream(QString name, std::function<void (QVariantMap)> & handler) {
    StreamConnector::instance()->addStreamSender(name.toStdString());
    handler = [name](QVariantMap data){
        //qDebug() << "sending data" << data["sound"].toByteArray().size();
        StreamConnector::instance()->sendData(data, name.toStdString());
    };
}
