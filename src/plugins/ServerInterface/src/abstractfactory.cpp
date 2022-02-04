#include "abstractfactory.h"

AbstractFactory::AbstractFactory()
{

}

AbstractSubsystem *AbstractFactory::produceItem(const std::string &name)
{
    auto result = itemProducers.find(name);
    if(result != itemProducers.end()) {
        return itemProducers[name]();
    }
    else return nullptr;
}

FactoryMap<AbstractSubsystem>::Map AbstractFactory::itemProducers = { };
