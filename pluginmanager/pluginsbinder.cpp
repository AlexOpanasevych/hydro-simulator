#include "pluginsbinder.h"

#include <QPluginLoader>
#include <QQmlEngine>
#include <QJsonDocument>
#include <QFile>

PluginsBinder::PluginsBinder(QObject *parent) : QObject(parent) {}

void PluginsBinder::setEngine(QQmlEngine *engine)
{
    this->engine = engine;
}

bool PluginsBinder::importPlugins()
{
    pluginsPath = getPathes(QUrl(importPath + "config.json").path());
    if (engine != nullptr)
    {
        engine->addImportPath(QUrl(importPath).path());
        for (auto path : pluginsPath)
        {
            QPluginLoader qmlPlugin(QUrl(path).path());
            qmlPlugin.load();
            if (!qmlPlugin.isLoaded())
            {
                qDebug() << "ERROR while opening plugin: " << qmlPlugin.errorString() << path;
            }
        }
    }
    return false;
}

QStringList PluginsBinder::getPathes(QString cfgPath)
{
    QFile jsonFile(cfgPath);
    if (!jsonFile.open(QIODevice::ReadOnly)) { qDebug() << "Error opening: " << jsonFile.errorString() << jsonFile.fileName(); return {}; }
    QByteArray saveData = jsonFile.readAll();
    QVariantList varList = (QJsonDocument::fromJson(saveData)).toVariant().toList();
    jsonFile.close();
    QStringList result;

    for (auto element : varList)
    {
        auto map = element.toMap();
        if (map["ACTIVE"].toBool())
        {
            result.push_back(map["PATH"].toString());
        }
    }

    return result;
}
