#ifndef BKPLUGIN_PLUGIN_H
#define BKPLUGIN_PLUGIN_H

#include <AbstractPluginInterface.h>
#include <QQmlExtensionPlugin>
#include "bk.h"

class BKPluginPlugin : public QQmlExtensionPlugin, public AbstractPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
    Q_INTERFACES(AbstractPluginInterface)
    QString m_appPath = "";
    AbstractEngine *m_engine = nullptr;
    BK * bk = nullptr;

public:
    void registerTypes(const char *uri) override;

    // AbstractPluginInterface interface
public:
    void init(AbstractEngine *engine, QString appPath, const QVariantList &depencies) override;
    ComponentsMap getComponents() override;
};

#endif // BKPLUGIN_PLUGIN_H
