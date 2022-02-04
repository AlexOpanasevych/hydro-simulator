#ifndef ABSTRACTFACTORY_H
#define ABSTRACTFACTORY_H

#include "abstractsubsystem.h"
#include "../serverinterface_global.h"
#include <string>
#include <functional>
#include <map>
#include <serverinterface_global.h>
#include <klibinfo.h>

template<typename T>
struct FactoryMap
{
    using Map = std::map<std::string, std::function<T*()>>;
};

class SERVERINTERFACE_LIBRARY_EXPORT AbstractFactory
{
public:
    AbstractFactory();

    template<typename T>
    //!
    //! \brief Registers new item in abstract factory
    //! \param name
    //!
    static void registerItem()
    {
        if(std::is_base_of<AbstractSubsystem, T>()) {
            itemProducers[std::string(type_name<T>())] = [](){
                return new T;
            };
        }
    }

    //!
    //! \brief produces item by given \p name
    //! \param name
    //! \return Created subsystem
    //!
    static AbstractSubsystem * produceItem(const std::string & name);

private:
    static FactoryMap<AbstractSubsystem>::Map itemProducers;

};

#endif // ABSTRACTFACTORY_H
