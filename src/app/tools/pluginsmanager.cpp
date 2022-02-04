#include "pluginsmanager.h"
#include <libs/PluginsBase/iphysicalitem.h>
#include <libs/PluginsBase/factories/controllerfactory.h>
#include <AbstractServerInterface.h>
#include <QPluginLoader>
#include <cdebug.h>
#include <scene/kscene.h>
#include <QDir>
#include <QFileInfoList>
#include <QJsonDocument>
#include <QLocale>
#include <engine.h>

PluginsManager::PluginsManager(QObject *parent) : QObject(parent)
{
    m_plugins = m_pluginsList.model();
    m_plugins->setParent(this);
}

PluginsManager::~PluginsManager()
{
    save();
}

void PluginsManager::setAppPath(QString appPath)
{
    QString result = appPath;
    setProgPath(appPath);
#ifdef QT_DEBUG
    qDebug(g) << "DEBUG MODE";
#ifdef Q_OS_LINUX
    int times = 4;
    setPluginsPath("/bin/plugins/");
#endif
#ifdef Q_OS_WINDOWS
    int times = 5;
    setPluginsPath("\\bin\\plugins\\");
#endif
#else
    qDebug(g) << "RELEASE MODE";
    int times = 2;
#ifdef Q_OS_LINUX
    setPluginsPath("/plugins/");
#endif
#ifdef Q_OS_WINDOWS
    setPluginsPath("\\plugins\\");
#endif
#endif

    while(times > 0 && !result.isEmpty()){
#ifdef Q_OS_LINUX
        if (result.endsWith("/")) times--;
#endif
#ifdef Q_OS_WINDOWS
        if (result.endsWith("\\")) times--;
#endif
        result.chop(1);
    }
    setRootPath(result);
}

void PluginsManager::setEngine(QQmlEngine *engine)
{
    m_engine = engine;
}

void PluginsManager::setScene(KScene *scene)
{
    m_scene = scene;
}

void PluginsManager::setCoreEngine(Engine *engine)
{
    m_coreEngine = engine;
}

void PluginsManager::load()
{
    QList<ControllerFactory *> controllers;
    QStringList createableTypes;
    getPathes(rootPath() + pluginsPath() + "config.json");

    qDebug(g) << "\033[39mImporting plugins " << rootPath() + pluginsPath() + "config.json";
    if (m_coreEngine != nullptr && m_scene != nullptr){
        m_scene->setEngine(m_engine);
        m_engine->addImportPath(rootPath() + pluginsPath());

        std::sort(m_pluginsList.begin(), m_pluginsList.end(), [](PluginData *obj1, PluginData *obj2) -> bool {
            return obj1->priority() < obj2->priority();
        });

        for (auto plugin : m_pluginsList){
            if (plugin->enabled()){
                QPluginLoader qmlPlugin(plugin->path());
                qmlPlugin.load();
                if (qmlPlugin.isLoaded()) {
                    QVariantList depencies = getDepencies(plugin->path());
                    qDebug(g) << "|=================================|";
                    qDebug(g) << "| Plugin: \033[33m" << plugin->name() << "\033[39m";
                    if (depencies.length() != 0){
                        qDebug(g) << "| Depencies: ";
                        for (auto a : depencies)  qDebug(g) << "| \033[33m" << a.toString() <<  "\033[39m";
                    }
                    AbstractPluginInterface* plugin = qobject_cast<AbstractPluginInterface*>(qmlPlugin.instance());
                    AbstractServerInterface* serverPlugin = qobject_cast<AbstractServerInterface*>(qmlPlugin.instance());
                    IPhysicalItem * piPlugin = qobject_cast<IPhysicalItem*>(qmlPlugin.instance());
                    if (piPlugin != nullptr){
                        piPlugin->init(m_coreEngine, rootPath(), depencies);
                        m_scene->addComponents(piPlugin->getComponents());
                        m_scene->addFactory(piPlugin->getSceneItemsFactories());
                        createableTypes += piPlugin->creatableTypes();
                        controllers.append(piPlugin->getControllerFactories());
                    }else if (plugin != nullptr){
                        plugin->init(m_coreEngine, rootPath(), depencies);
                        m_scene->addComponents(plugin->getComponents());
                    }else if(serverPlugin != nullptr){
                        serverPlugin->init(m_coreEngine, rootPath(), depencies);
                        m_scene->addComponents(serverPlugin->getComponents());
                    }
                }else{
                    qDebug(g) << "\033[91m\033[40m ERROR while opening plugin: " << qmlPlugin.errorString() << plugin->path() <<  "\033[39m\033[49m";
                }
            }
        }
        qDebug(g) << "|=================================|";
    }

    QHash<QString, ControllerFactory *> hControllerFactory;
    for (auto item : controllers){
        hControllerFactory.insert(item->objectName(), item);
    }
    m_coreEngine->setControllers(hControllerFactory);
    m_coreEngine->setCreatebleTypes(createableTypes);
}

int PluginsManager::priority(QString name)
{
    for (const auto &pl : m_pluginsList){
        if (pl->name() == name) return pl->priority();
    }
    return -1;
}

void PluginsManager::save()
{
    QFile jsonFile(rootPath() + pluginsPath() + "config.json");
    jsonFile.open(QIODevice::WriteOnly);
    QVariantList data;

    for (const auto &pl : m_pluginsList){
        QVariantMap m;
        m["ENABLED"] = pl->enabled();
        m["DATEMODIFIED"] = pl->dateModified();
        m["PRIORITY"] = pl->priority();
        m["PATH"] = pl->path();
        m["NAME"] = pl->name();
        data.push_back(m);
    }
    jsonFile.write(QJsonDocument::fromVariant(data).toJson());
    jsonFile.close();
}

void PluginsManager::getPathes(QString path)
{
    QFile jsonFile(path);
    if (!jsonFile.open(QIODevice::ReadOnly)) {
        qDebug(v) << "Error opening: " << jsonFile.errorString() << path;
        qDebug(v) << "Creating new config.json...";
        createNewConfig();
        return;
    }
    QByteArray saveData = jsonFile.readAll();
    QVariantList varList = (QJsonDocument::fromJson(saveData)).toVariant().toList();
    jsonFile.close();

    for (int i = m_pluginsList.count() - 1; i >= 0; i--){
        m_pluginsList.takeAt(i)->deleteLater();
    }
    m_pluginsNames.clear();
    for (auto element : varList)
    {
        auto map = element.toMap();
        auto pl = new PluginData(this);
        connect(pl, &PluginData::enabledChanged, this, [this, pl](bool value){
            if (value){
                if (!m_availablePlugins.contains(pl->name())){
                    m_availablePlugins.push_back(pl->name());
                }
            }else{
                m_availablePlugins.removeAll(pl->name());
            }
        });
        pl->setName(map["NAME"].toString());
        pl->setEnabled(map["ENABLED"].toBool());
        pl->setDateModified(map["DATEMODIFIED"].toString());
        pl->setPriority(map["PRIORITY"].toInt());
        pl->setPath(map["PATH"].toString());
        m_pluginsNames.push_back(pl->name());
        m_pluginsList.push_back(pl);
    }
}

void PluginsManager::createNewConfig()
{
    pluginsCount = 1;
    for (int i = m_pluginsList.count() - 1; i >= 0; i--){
        m_pluginsList.takeAt(i)->deleteLater();
    }
    m_pluginsNames.clear();
    getSubElements(rootPath() + pluginsPath());
}

QVariantList PluginsManager::getDepencies(QString pluginPath)
{
    QVariantList result;
    QString filePath;
    int times = 1;
    while(times > 0 && !pluginPath.isEmpty()){
#ifdef Q_OS_LINUX
        if (pluginPath.endsWith("/")) times--;
#endif
#ifdef Q_OS_WINDOWS
        if (result.endsWith("\\")) times--;
#endif
        pluginPath.chop(1);
    }

    QFile jsonFile(pluginPath + "/depencies.json");
    if (jsonFile.open(QIODevice::ReadOnly)){
        QByteArray saveData = jsonFile.readAll();
        result = (QJsonDocument::fromJson(saveData)).toVariant().toList();
        jsonFile.close();

        for (int i = result.count() - 1; i >=0; i--){
            QString name = result[i].toString();
            if (!m_availablePlugins.contains(name)) result.removeAll(QVariant(name));
        }
    }
    return result;
}

void PluginsManager::getSubElements(QString path)
{
    QDir dir(path);
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QString pluginPath = "", pluginName = "", dateModified = "";

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

        if (fileInfo.isDir())
        {
            getSubElements(fileInfo.filePath());
        }
        else {
            if(fileInfo.fileName() == "qmldir") {
                QFile qmldirFile(fileInfo.absoluteFilePath());
                if(!qmldirFile.open(QFile::ReadOnly)) qDebug(g) << "Cannot open file" << fileInfo.fileName() << qmldirFile.errorString();
                QString moduleString = qmldirFile.readAll();
                QRegularExpression reg("(?i)(?:module\\s(\\w+)?)");
                auto match = reg.match(moduleString);
                pluginName = match.captured(1);
                qDebug() << "pluginName" << pluginName;
                pluginPath = fileInfo.dir().absolutePath() + '/' + pluginName;

                dateModified = fileInfo.lastModified().toString("dd.MM.yyyy|hh:mm:ss");
                qmldirFile.close();
#ifdef WIN64
                pluginPath += ".dll";
#else
                pluginPath += ".so";
#endif
                break;
            }

//            else if (fileInfo.fileName() == "qmldir" && fileInfo.isFile()) {
//                QFile file(fileInfo.canonicalFilePath());
//                if (file.open(QIODevice::ReadOnly) )
//                {
//                    QTextStream data(&file);
//                    QString f = data.readLine().split(" ")[1];
//                    pluginName = f;
//                    dateModified = fileInfo.lastModified().toString("dd.MM.yyyy|hh:mm:ss");
//                    file.close();
//                }
//            }
        }
    }
    if (pluginName != "")
    {
        auto pl = new PluginData(this);
        connect(pl, &PluginData::enabledChanged, this, [this, pl](bool value){
            if (value){
                if (!m_availablePlugins.contains(pl->name())){
                    m_availablePlugins.push_back(pl->name());
                }
            }else{
                m_availablePlugins.removeAll(pl->name());
            }
        });
        pl->setDateModified(dateModified);
        pl->setPriority(pluginsCount++);
        pl->setPath(pluginPath);
        pl->setName(pluginName);
        pl->setEnabled(true);
        m_pluginsNames.push_back(pl->name());
        m_pluginsList.push_back(pl);
    }
}
