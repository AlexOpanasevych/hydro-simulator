#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <QObject>
#include <QStringList>
//#include <libs/klibcorelite/kmacro.h>
#include <libs/klibcorelite/models/kflexiblemodel.h>

class PluginManager : public QObject
{
    Q_OBJECT
    K_QML_TYPE(PluginManager)
    K_CONST_PROPERTY(QString, pluginsPath, "../../bin/plugins/")
    K_AUTO_PROPERTY(QString, choosenPath, choosenPath, setChoosenPAth, choosenPAthChanged, "")
    K_READONLY_PROPERTY(KFlexibleModel *, model, model, setModel, modelChanged, new KFlexibleModel(this))
    K_READONLY_PROPERTY(QStringList, pathes, pathes, setPathes, pathesChanged, QStringList())
public:
    explicit PluginManager(QObject *parent = nullptr);
public slots:
    void loadConfig();
    void save();

signals:

private:
    QVariantList getSubElements(QString path = "");
    QVariant readConfig();
    void updateModel(const QVariant &variant);
    void updateList();
};

#endif // PLUGINMANAGER_H
