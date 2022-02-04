#ifndef SOUNDFACTORY_H
#define SOUNDFACTORY_H

#include <sound.h>

template<typename T>
struct FactoryMap
{
    using Map = std::map<QString, std::function<T*()>>;
};

//!
//! \brief The SoundFactory class
//!
class SoundFactory
{
public:
    SoundFactory();

    template<typename T>
    //!
    //! \brief Register new sound class in factory
    //! \param name
    //!
    static void registerSound(const QString &name)
    {
        if(std::is_base_of<Sound, T>()) {
            itemProducers[name] = [name](){
                auto sound = new T;
                sound->setObjectName(name);
                return sound;
            };
        }
    }
    static Sound * createSound(QString &name);
private:
    static FactoryMap<Sound>::Map itemProducers;
};

#endif // SOUNDFACTORY_H
