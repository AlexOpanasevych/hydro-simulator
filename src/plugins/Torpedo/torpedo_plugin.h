#ifndef TORPEDO_PLUGIN_H
#define TORPEDO_PLUGIN_H

#include <QQmlExtensionPlugin>
#include <QQmlComponent>
#include <libs/PluginsBase/iphysicalitem.h>
#include <libs/PluginsBase/base/sceneitem.h>
#include <libs/PluginsBase/factories/sceneitemsfactory.h>
#include <libs/PluginsBase/factories/controllerfactory.h>
#include <sceneItems/torpedo.h>
#include <engine/controllers/torpedocontroller.h>

class TorpedoPlugin : public QQmlExtensionPlugin, public IPhysicalItem
{
    Q_OBJECT
    Q_INTERFACES(IPhysicalItem)
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
    virtual void init(AbstractEngine *engine, QString appPath = "", const QVariantList &depencies = QVariantList()) override;
    virtual ComponentsMap getComponents() override;
    virtual QStringList creatableTypes() override;
    void initSound(QString soundName, int uniqueId, QVariantMap data);
    void stopSound(int id, QString name = "");
private:
    QList<SceneItemsFactory *> creators;
    QList<ControllerFactory *> controllers;
    AbstractEngine *engine = nullptr;
#ifdef QT_NO_DEBUG
    QString configPath = "/plugins/Torpedo/content/";
#else
    QString configPath = "/bin/plugins/Torpedo/content/";
#endif
    QObject * audioModulePlugin = nullptr;
    QString m_appPath = "";
};

#endif // TORPEDO_PLUGIN_H
