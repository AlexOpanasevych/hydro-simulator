#include "mgkitemsfactory.h"

MGKItemsFactory::MGKItemsFactory(QObject *parent) : QObject(parent)
{

}

MGKItem *MGKItemsFactory::produceItem(const QString &name)
{
    auto result = itemProducers.find(name);
    if(result != itemProducers.end()) {
        return itemProducers[name]();
    }
    else return nullptr;
}

QMap<QString, std::function<MGKItem*()>> MGKItemsFactory::itemProducers = { };
