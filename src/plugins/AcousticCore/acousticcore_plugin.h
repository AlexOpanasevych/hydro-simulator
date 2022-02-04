#ifndef WAVECORE_PLUGIN_H
#define WAVECORE_PLUGIN_H

#include <QQmlExtensionPlugin>
#include <AbstractPluginInterface.h>

class AcousticCorePlugin : public QQmlExtensionPlugin, public AbstractPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
    Q_INTERFACES(AbstractPluginInterface)

public:
    void registerTypes(const char *uri) override;
    virtual void init(AbstractEngine *engine, QString appPath = "", const QVariantList &depencies = QVariantList()) override;
    virtual ComponentsMap getComponents() override;
private:
    AbstractEngine *m_engine = nullptr;
    QObject * audioModulePlugin = nullptr;
    QString m_appPath = "";
};

#endif // WAVECORE_PLUGIN_H


