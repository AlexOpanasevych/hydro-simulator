#include "updateservice.h"
#include <QDebug>
#include <QDir>
#include <QFile>

UpdateService::UpdateService(QObject * parent) : QObject(parent), m_provider("mgk", "192.168.81.201", "mgk", "Br0nzaPla")
{
    m_provider.setSchema("dbo");
}

void UpdateService::remove(const QString & name, int version)
{

}

void UpdateService::removeAll()
{

}

void UpdateService::update(const QString &name, const QString &path)
{
    // update plugin with desired name, path

}

UpdateService::~UpdateService()
{
    if(!m_pluginsList.empty()) {
        qDeleteAll(m_pluginsList);
        m_pluginsList.clear();
    }
}

QList<QObject*> UpdateService::loadAll()
{
    if(!m_pluginsList.isEmpty()) {
        for(auto && plugin : m_pluginsList) {
            plugin->deleteLater();
        }
        m_pluginsList.clear();
    }
    m_provider.setTable("dll");
    auto result = m_provider.selectJoin({"MAX(date)", "name", "MAX(version)", "dll"}, "plugins", SSMSProvider::JoinType::INNER, "pluginId", "id").get();
    if(!result.isEmpty()) {
        QDir currDirectory;
        if(!currDirectory.entryList(QDir::Dirs).contains(path))
            currDirectory.mkdir(path);
        currDirectory.cd(path);
        for(auto && item : result) {
            auto pluginPath = currDirectory.absolutePath() + "/" + item["name"].toString() + ".dll";
            if(!currDirectory.entryList(QDir::Files).contains(item["name"].toString() + ".dll")) {
                QFile file(pluginPath);
                if(!file.open(QFile::WriteOnly)) {
                    qDebug() << "can't create file:" << pluginPath;
                }
                auto data = item["dll"].toByteArray();
                file.write(data.data(), data.size());
                file.close();
            }
//            auto pluginPath = QString("./") + path + item["name"].toString() + ".dll";
            QPluginLoader loader(pluginPath);

            loader.load();

            if(loader.isLoaded()) {
                m_pluginsList.append(loader.instance());
            }
            else {
                qDebug() << "not loaded" << pluginPath << loader.errorString();
            }
        }
        qDebug() << m_pluginsList;
    }
    return m_pluginsList;
}

QList<QObject*> UpdateService::pluginsList() const
{
    return m_pluginsList;
}

QObject *UpdateService::load(const QString &name)
{
    m_provider.setTable("dll");
    auto result = m_provider.selectJoin({"MAX(date)", "name", "MAX(version)", "dll"}, "plugins", SSMSProvider::JoinType::INNER, "pluginId", "iddll").where({"name", name}).get();
    if(result.size() > 1) {
        qDebug() << "found more than one plugin with name" << name;
        return nullptr;
    }
    auto pluginPath = path + result[0]["name"].toString();
    QFile file(pluginPath);
    if(!file.open(QFile::WriteOnly)) {
        qDebug() << "can't create file:" << result[0]["name"];
    }
    auto data = result[0]["dll"].toByteArray();
    file.write(data.data(), data.size());
    file.close();

    QPluginLoader loader(pluginPath);

    loader.load();
    if(loader.isLoaded() && m_pluginsList.contains(loader.instance())) {
        m_pluginsList.append(loader.instance());
    }
    else {
        qDebug() << "not loaded" << pluginPath;
    }
    return loader.instance();
}
