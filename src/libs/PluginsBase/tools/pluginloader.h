#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include <QDir>
#include <QObject>
#include <QPluginLoader>
#include <QString>
#include <QUrl>
#include <cdebug.h>

static QObject* pluginloader(QString libname, QString appPath){
    auto d = QDir::current();
#ifdef QT_NO_DEBUG
    d.cd( appPath + "/plugins/" + libname + "/");
#else
    d.cd( appPath + "/bin/plugins/" + libname + "/");
#endif
#ifdef WIN64
    QString path = QUrl(d.absolutePath() + "/" + libname + ".dll").path();
#else
    QString path = QUrl(d.absolutePath() + "/lib" + libname + ".so").path();
#endif
    QPluginLoader qmlPlugin(path);
    qmlPlugin.load();
    if (!qmlPlugin.isLoaded()) {
        qDebug(v) << "ERROR while opening plugin: " << qmlPlugin.errorString() << path;
    }
    return qmlPlugin.instance();
}

#endif // PLUGINLOADER_H
