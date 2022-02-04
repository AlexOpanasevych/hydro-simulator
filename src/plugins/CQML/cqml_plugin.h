#ifndef CQML_PLUGIN_H
#define CQML_PLUGIN_H

#include <QQmlExtensionPlugin>

class CQMLPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
};

#endif // CQML_PLUGIN_H
