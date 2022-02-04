#ifndef MGKITEMSFACTORY_H
#define MGKITEMSFACTORY_H

#include "mgkitembase.h"

#include <QObject>

class MGKItemsFactory : public QObject
{
    Q_OBJECT
public:
    explicit MGKItemsFactory(QObject *parent = nullptr);

    template<typename T>
    static void registerItem(const QString &name, QObject * parent);

    static MGKItem * produceItem(const QString & name);

signals:

private:
    static QMap<QString, std::function<MGKItem*()>> itemProducers;

};

template<typename T>
void MGKItemsFactory::registerItem(const QString &name, QObject * parent)
{
    itemProducers[name] = [parent](){
        return new T(parent);
    };
}

#endif // MGKITEMSFACTORY_H
