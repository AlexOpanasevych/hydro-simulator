#ifndef SCENEITEMS_PLUGIN_H
#define SCENEITEMS_PLUGIN_H

#include <QQmlComponent>
#include <QQmlExtensionPlugin>
#include <libs/PluginsBase/iphysicalitem.h>
#include <libs/PluginsBase/base/sceneitem.h>
#include <libs/PluginsBase/factories/sceneitemsfactory.h>
#include <libs/PluginsBase/factories/controllerfactory.h>

class VesselPlugin : public QQmlExtensionPlugin, public IPhysicalItem
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
    void stopSound(int id = -1, QString name = "");
private:
    AbstractEngine *engine = nullptr;
#ifdef QT_NO_DEBUG
    QString configPath = "/plugins/Vessel/content/";
#else
    QString configPath = "/bin/plugins/Vessel/content/";
#endif
    QObject * audioModulePlugin = nullptr;
    QString m_appPath = "";
};

#endif // SCENEITEMS_PLUGIN_H
