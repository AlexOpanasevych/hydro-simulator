#include "updateservice.h"

#include <QDateTime>

UpdateService::UpdateService(QObject *parent) : QObject(parent), m_provider("mgk", "192.168.81.201", "mgk", "Br0nzaPla")
{
    pluginsDirectory.setPath("../../bin/plugins");
    m_provider.setSchema("dbo");
}

void UpdateService::updatePlugins(QString author, int version, QString description)
{
    m_provider.setTable("plugins");
    auto names = m_provider.select({"name"}).get();
    qDebug() << names;
    // todo : add versions support
    auto pList = pluginPaths();

    for (int i = 0; i < pList.size(); i++) {
        if(!names.contains({{"name", pList[i]}})) {
            qDebug() << "new" << pList[i];
            m_provider.insertRow({{"name", pList[i]}, {"description", description.isEmpty() ? "No description" : description}, {"author", author.isEmpty() ? "No author" : author}});
        }
    }

    auto ids = m_provider.select().get();
    qDebug() << ids;
    m_provider.setTable("dll");
    for(int i = 0; i < pList.size(); i++) {
        // go into plugin directory
        pluginsDirectory.cd(pList[i]);
        // open dll file
        QFile dllFile(pluginsDirectory.path() + "/" + pList[i] + ".dll");
        if(!dllFile.open(QFile::ReadOnly)) return;
        qDebug() << "opened:" << dllFile.fileName() << "insert rows";
        m_provider.insertRow({{"date", QDateTime::currentDateTime().toString(Qt::ISODate)}, {"version", version != -1 ? version : 0}, {"dll", dllFile.readAll()}, {"pluginId", ids[i]["id"]}});
        pluginsDirectory.cdUp();
    }
    emit pluginsUpdated();
}

QList<QString> UpdateService::pluginPaths()
{
    return pluginsDirectory.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
}
