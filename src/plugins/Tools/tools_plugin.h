#ifndef TOOLS_PLUGIN_H
#define TOOLS_PLUGIN_H

#include <QQmlExtensionPlugin>
//#include <libs/PluginsBase/externaiInterface.h>
#include <AbstractPluginInterface.h>

class ToolsPlugin : public QQmlExtensionPlugin, public AbstractPluginInterface//, public IPhysicalItem
{
    Q_OBJECT
    Q_INTERFACES(AbstractPluginInterface)
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    virtual void registerTypes(const char *uri) override;
    virtual void init(AbstractEngine * engine, QString appPth = "", const QVariantList &depencies = QVariantList()) override;
    virtual ComponentsMap getComponents() override;

    ToolsPlugin();
    ~ToolsPlugin();

private:
    ComponentsMap m_components;
    QString m_appPath = "";
};



#endif // TOOLS_PLUGIN_H
