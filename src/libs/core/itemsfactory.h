#ifndef ITEMSFACTORY_H
#define ITEMSFACTORY_H

#include "abstractpicontroller.h"
#include "core_global.h"
class CORE_LIBRARY_EXPORT ItemsFactory
{
public:
    ItemsFactory();
    std::map<std::string, std::function<AbstractPIController*()>> m_controllersCreator;

    //!
    //! \brief Creates controller which is created with \a m_controllersCretors[type]
    //! \return Created controller
    //!
    AbstractPIController * createController(std::string);

    //!
    //! \brief Register controller creator callback
    //! \param controller
    //!
    void registerController(std::string, std::function<AbstractPIController*()> controller);

    //    std::function<void(std::string)>
};



#endif // ITEMSFACTORY_H
