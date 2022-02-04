#include "pluginmanager.h"
#include <QDateTime>
#include <QDir>
#include <QJsonDocument>
#include <QVariant>

PluginManager::PluginManager(QObject *parent) : QObject(parent)
{
    loadConfig();
}

void PluginManager::loadConfig()
{
    model()->clear();
    auto jsonCfg = readConfig();
    model()->deserialize(jsonCfg, false, true);
    if (model()->sortedRoleNames().length() == 0)
        model()->addRoles({"NAME", "DATEMODIFIED", "PATH", "ACTIVE"});

    updateList();
}

QVariant PluginManager::readConfig()
{
    QFile jsonFile(pluginsPath() + "config.json");
//    qDebug() << QDir().absoluteFilePath(jsonFile.fileName());
    if (!jsonFile.open(QIODevice::ReadWrite)) { qDebug() << "Error opening: " << jsonFile.errorString() << pluginsPath(); return {}; }
    QByteArray saveData = jsonFile.readAll();
    QVariant var = QJsonDocument::fromJson(saveData).toVariant();
    jsonFile.close();
    return var;
}

void PluginManager::updateModel(const QVariant &variant)
{
    QVariantList list = variant.toList();

    for (auto element : list)
    {
        auto map = element.toMap();
        if (model()->indexOf(map["NAME"], "NAME") == -1)
        {
            model()->push_back(map);
        }
        else
        {
            auto idx = model()->indexOf(map["NAME"], "NAME");
            QVariantMap m;
            m.insert("DATEMODIFIED", map["DATEMODIFIED"]);
            m.insert("PATH", map["PATH"]);
            model()->setLineData(m, idx);
        }
    }
}

void PluginManager::updateList()
{
    setPathes(QStringList());
    QVariant v = getSubElements(pluginsPath());
    updateModel(v);
    save();
}

void PluginManager::save()
{
    QFile jsonFile(pluginsPath() + "config.json");
    if (!jsonFile.open(QIODevice::WriteOnly)) { qDebug() << "Error opening: " << jsonFile.errorString(); return; }
    QVariant data = model()->serialize();
    jsonFile.write(QJsonDocument::fromVariant(data.toList()).toJson());
    jsonFile.close();
}

QVariantList PluginManager::getSubElements(QString path)
{
    QDir dir(QUrl(path).path());
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QString pluginPath = "", pluginName = "", dateModified = "";
    QVariantList result{};

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

        if (fileInfo.isDir())
        {
            QVariant var = getSubElements(fileInfo.filePath());
            if (var.isValid() && !var.isNull())
            {
                result.append(var.toList());
            }
        }
        else {
#ifdef WIN64
            if (fileInfo.fileName().contains(".dll")) pluginPath = fileInfo.canonicalFilePath();
            else if (fileInfo.fileName() == "qmldir" && fileInfo.isFile()) {
                QFile file(fileInfo.canonicalFilePath());
                if (file.open(QIODevice::ReadOnly) )
                {
                    QTextStream data(&file);
                    QString f = data.readLine().split(" ")[1];
                    pluginName = f;
                    dateModified = fileInfo.lastModified().toString(Qt::SystemLocaleShortDate);
                    file.close();
                }
            }
#else
            if (fileInfo.fileName().contains(".so")) pluginPath = fileInfo.canonicalFilePath();
            else if (fileInfo.fileName() == "qmldir" && fileInfo.isFile()) {
                QFile file(fileInfo.canonicalFilePath());
                if (file.open(QIODevice::ReadOnly) )
                {
                    QTextStream data(&file);
                    QString f = data.readLine().split(" ")[1];
                    pluginName = f;
                    dateModified = fileInfo.lastModified().toString(Qt::SystemLocaleShortDate);
                    file.close();
                }
            }
#endif
        }
    }
    if (pluginName != "")
    {
        QVariantMap m{
            {"NAME", pluginName},
            {"DATEMODIFIED", dateModified},
            {"PATH", pluginPath},
            {"ACTIVE", true}
        };
        result.append(m);
    }
    //model()->push_back({pluginName, dateModified, pluginPath, true});
    //m_pathes.push_back(pluginPath);
    return result;
}