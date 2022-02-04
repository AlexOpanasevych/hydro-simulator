#include "ksoundarray.h"


//!
//! \brief Get all sounds
//! \return sounds
//!
QList<Sound*> KSoundArray::sounds() const {
    QList<Sound*> result;
    for(auto && soundMix : m_sounds) {
        result.append(soundMix);
    }
    return result;
}

//!
//! \brief Constructor
//! \param play
//! \param parent
//!
KSoundArray::KSoundArray(bool play, QObject *parent) : Sound(play, parent) {
    connect(this, &Sound::sampleRateChanged, this, [this](qint64 value){
        for(auto && item : m_sounds) {
            for(Sound *sound : item) {
                sound->setSampleRate(value);
            }
        }
    });
}

//!
//! \brief Add sound vector for desired \p id
//! \param id
//! \param vectorSound
//!
void KSoundArray::addSound(int id, QVector<Sound *> *vectorSound)
{
    for(auto sound = vectorSound->constBegin(); sound < vectorSound->constEnd(); ++sound)
    {
        (*sound)->setParent(this);
        m_sounds[id].push_back(*sound);
        emit soundAdded(*sound);
    }
}

//!
//! \brief Adds \p sound for desired \p id
//! \param id
//! \param sound
//!
void KSoundArray::addSound(int id, Sound *sound) {
    overrideAddSound(sound);
    sound->setParent(this);
    m_sounds[id].push_back(sound);
    emit soundAdded(sound);
}

//!
//! \brief Get size of array with all \a id's
//! \return size of sound array
//!
int KSoundArray::size()
{
    int size = 0;
    for(auto && soundId = m_sounds.begin(); soundId != m_sounds.end(); soundId++) {
        size += soundId.value().size();
    }
    return size;
}

//!
//! \brief Removes \p sound passed by pointer for desired \p id
//! \param id
//! \param sound
//!
void KSoundArray::removeSound(int id, Sound *sound) {
    sound->setParent(nullptr);
    m_sounds[id].removeOne(sound);
    sound->deleteLater();
    emit soundRemoved(sound);
}

//!
//! \brief Removes sound with \p type and \p id
//! \param id
//! \param type
//!
void KSoundArray::removeSound(int id, QString type)
{
    if(id != -1) {
        auto search = m_sounds.find(id);
        if(search != m_sounds.end()) {
            if(!type.isEmpty()) {
                for(auto && sound : *search) {
                    if(sound->objectName() == type) {
                        this->removeSound(id, sound);
                    }
                }
            }
            else {
                for(auto && sound : *search) {
                    this->removeSound(id, sound);
                }
            }
        }
    }
    else if(id == -1){
        for(auto && soundArr : m_sounds) {
            for(auto && sound : soundArr) {
                sound->setParent(nullptr);
                sound->deleteLater();
                emit soundRemoved(sound);
            }
        }
        m_sounds.clear();
    }
}
