#ifndef SAVEPROVIDER_PLUGIN_H
#define SAVEPROVIDER_PLUGIN_H

#include <QQmlExtensionPlugin>
#include <QQmlComponent>
#include <AbstractPluginInterface.h>

class SaveProviderPlugin : public QQmlExtensionPlugin, public AbstractPluginInterface
{
    Q_OBJECT
    Q_INTERFACES(AbstractPluginInterface)
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    virtual void registerTypes(const char *uri) override;
    virtual void init(AbstractEngine *engine, QString appPath = "", const QVariantList &depencies = QVariantList()) override;
    virtual ComponentsMap getComponents() override;
private:
    ComponentsMap m_components;
    QString m_appPath = "";
};

#endif // SAVEPROVIDER_PLUGIN_H
