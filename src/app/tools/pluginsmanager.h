#ifndef PLUGINSMANAGER_H
#define PLUGINSMANAGER_H

#include <QObject>
#include <kmacro.h>
#include <utility/kobservablelist.h>

class PluginData: public QObject
{
    Q_OBJECT
    K_READONLY_PROPERTY(QString, path, path, setPath, pathChanged, "")
    K_READONLY_PROPERTY(QString, name, name, setName, nameChanged, "")
    K_AUTO_PROPERTY(int, priority, priority, setPriority, priorityChanged, 0)
    K_AUTO_PROPERTY(bool, enabled, enabled, setEnabled, enabledChanged, false)
    K_READONLY_PROPERTY(QString, dateModified, dateModified, setDateModified, dateModifiedChanged, "")
    friend class PluginsManager;
public:
    PluginData(QObject *parent = nullptr) : QObject(parent) { }
};

class KScene;
class Engine;
class PluginsManager : public QObject
{
    Q_OBJECT
    K_SINGLETON(PluginsManager)
    K_QML_SINGLETON(PluginsManager)
    K_AUTO_PROPERTY(QString, rootPath, rootPath, setRootPath, rootPathChanged, "")
    K_READONLY_PROPERTY(QVariantList, pluginsNames, pluginsNames, setPluginsNames, pluginsNamesChanged, QVariantList())
    K_READONLY_PROPERTY(QVariantList, availablePlugins, avaliablePlugins, setAvailablePlugins, availablePluginsChanged, QVariantList())
    K_READONLY_PROPERTY(KObservableModel*, plugins, plugins, setPlugins, pluginsChanged, nullptr)
    K_AUTO_PROPERTY(QString, progPath, progPath, setProgPath, progPathChanged, "")
    K_READONLY_PROPERTY(QString, pluginsPath, pluginsPath, setPluginsPath, pluginsPathChanged, "")
public:
    explicit PluginsManager(QObject *parent = nullptr);
    ~PluginsManager();
    void setAppPath(QString appPath);
    void setEngine(QQmlEngine *engine);
    void setScene(KScene *scene);
    void setCoreEngine(Engine *engine);
    void load();
    int priority(QString name);
    Q_INVOKABLE void save();
signals:
private:
    QQmlEngine *m_engine = nullptr;
    KScene *m_scene = nullptr;
    Engine *m_coreEngine = nullptr;
    void getPathes(QString path);
    void createNewConfig();
    QVariantList getDepencies(QString pluginPath);
    void getSubElements(QString path);
    KObservableList<PluginData> m_pluginsList;
    int pluginsCount = 1;
};

#endif // PLUGINSMANAGER_H
