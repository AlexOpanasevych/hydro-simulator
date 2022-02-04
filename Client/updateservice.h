#ifndef MYSQLUPDATESERVICE_H
#define MYSQLUPDATESERVICE_H

#include "ipluginupdateservice.h"
#include <QPluginLoader>
#include <QQmlEngine>
#include <kmacro.h>
#include <ssmsprovider.h>

class UpdateService : public QObject, public IPluginUpdateService
{
    Q_OBJECT
    K_QML_TYPE(UpdateService)
public:
    explicit UpdateService(QObject * parent = nullptr);
    void remove(const QString & name, int version) override;
    void removeAll() override;
    void update(const QString & name, const QString & path) override;
    ~UpdateService();
    QList<QObject *> pluginsList() const;
    QList<QObject *> loadAll() override;

private:
    QObject * load(const QString & name) override;
private:
    SSMSProvider m_provider;
    QQmlEngine *engine = nullptr;
    QList<QObject*> m_pluginsList;
    const char * path = "plugins/";
};

#endif // MYSQLUPDATESERVICE_H
