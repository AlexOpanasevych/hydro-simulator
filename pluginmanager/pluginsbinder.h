#ifndef PLUGINSBINDER_H
#define PLUGINSBINDER_H

#include <QDir>
#include <QObject>
#include <QUrl>
class QQmlEngine;
class PluginsBinder : public QObject
{
    Q_OBJECT
public:
    explicit PluginsBinder(QObject *parent = nullptr);
    void setEngine(QQmlEngine *engine);
    bool importPlugins();

signals:

private:
    QQmlEngine *engine = nullptr;
    QStringList pluginsPath;
    QStringList getPathes(QString cfgPath);
    QString importPath = QDir::currentPath() + "/../../bin/plugins/";
};


#endif // PLUGINSBINDER_H
