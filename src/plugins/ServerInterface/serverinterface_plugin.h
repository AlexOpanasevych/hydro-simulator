#ifndef SERVERINTERFACE_PLUGIN_H
#define SERVERINTERFACE_PLUGIN_H

#include <QQmlExtensionPlugin>
#include <AbstractServerInterface.h>
#include <base/global_messages/createpimsg.h>
#include <cdebug.h>
class AbstractEngine;

class ServerInterfacePlugin : public QQmlExtensionPlugin, public AbstractServerInterface
{
    Q_OBJECT
    Q_INTERFACES(AbstractServerInterface)
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    //!
    //! \brief Register QML types
    //! \param uri
    //!
    void registerTypes(const char *uri) override;

    //!
    //! \brief Init plugin
    //! \param eng
    //!
    void init(AbstractEngine *eng, QString appPath = "", const QVariantList &depencies = QVariantList()) override;

    //!
    //! \brief Get components
    //! \return list of QML components
    //!
    ComponentsMap getComponents() override;

    //!
    //! \brief Get engine instance
    //! \return engine instance
    //!
    AbstractEngine *engine() const;

    //!
    //! \brief Registers stream
    //! \param name
    //! \param handler
    //!
    void registerStream(QString name, std::function<void (QVariantMap)> &) override;

    void testMsgHandler(CreatePIMsgAns msg) {qDebug(v) << "msg" << msg.createdId;}

private:
    AbstractEngine * m_engine;
    QString m_appPath = "";
//    const ComponentsMap m = {{"tools",{ {"serverInterface", {{"settings", "path/to/qml"} } }}}};
};

#endif // SERVERINTERFACE_PLUGIN_H
