#ifndef WAVE_PLUGIN_H
#define WAVE_PLUGIN_H

#include <AbstractPluginInterface.h>
#include <QQmlExtensionPlugin>

class WaveGenerator;

class WavePlugin : public QQmlExtensionPlugin, public AbstractPluginInterface {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)
    Q_INTERFACES(AbstractPluginInterface)
    AbstractEngine * engine;
    WaveGenerator * generator = nullptr;
    QObject * audioModulePlugin = nullptr;
public:
    void registerTypes(const char *uri) override;
    void init(AbstractEngine * engine, QString appPath = "", const QVariantList &depencies = QVariantList()) override;
    ComponentsMap getComponents() override;
    void setEngine(AbstractEngine *value);
    QString m_appPath = "";
};

#endif // WAVE_PLUGIN_H
