#include "itemsfactory.h"

ItemsFactory::ItemsFactory()
{

}



AbstractPIController *ItemsFactory::createController(std::string cName)
{
    auto search = m_controllersCreator.find(cName);
    if(search != m_controllersCreator.end())
        return search->second();
    return nullptr;
}

void ItemsFactory::registerController(std::string cType, std::function<AbstractPIController* ()> controller)
{
    m_controllersCreator.insert({cType, controller});
}
