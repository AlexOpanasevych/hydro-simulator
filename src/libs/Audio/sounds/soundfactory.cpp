#include "soundfactory.h"

SoundFactory::SoundFactory()
{

}

//!
//! \brief Creates new sound followed by \a name parameter
//! \param name
//! \return generated sound
//!
Sound *SoundFactory::createSound(QString &name)
{
    auto result = itemProducers.find(name);
    if(result != itemProducers.end()) {
        return itemProducers[name]();
    }
    else return nullptr;
}

FactoryMap<Sound>::Map SoundFactory::itemProducers = { };
