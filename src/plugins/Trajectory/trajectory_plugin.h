#ifndef TRAJECTORY_PLUGIN_H
#define TRAJECTORY_PLUGIN_H

#include <QQmlExtensionPlugin>

class TrajectoryPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
};

#endif // TRAJECTORY_PLUGIN_H
