#ifndef IPLUGINUPDATESERVICE_H
#define IPLUGINUPDATESERVICE_H

#include <QObject>
#include <QString>


class IPluginUpdateService
{
public:
    IPluginUpdateService(){}
    virtual void update(const QString & name, const QString & filename) = 0;
    virtual void remove(const QString & name, int version) = 0;
    virtual void removeAll() = 0;
    virtual QList<QObject*> loadAll() = 0;
    virtual QObject * load(const QString & name) = 0;
    virtual ~IPluginUpdateService(){}
};


#endif // IPLUGINUPDATESERVICE_H
