#include "ksoundchain.h"

//!
//! \brief KSoundChain::interval
//! \return
//!
qint64 KSoundChain::interval() const {
    return m_interval;
}

//!
//! \brief KSoundChain::setInterval
//! \param interval
//!
void KSoundChain::setInterval(const qint64 &interval) {
    m_interval = interval;
}

//!
//! \brief KSoundChain::switchSound
//!
void KSoundChain::switchSound() {
    if(currentSound) {
        int index = sounds().indexOf(currentSound);
        if(++index < sounds().size()) {
            currentSound = sounds().at(index);
        } else {
            currentSound = nullptr;
        }
    }
}

//!
//! \brief KSoundChain::KSoundChain
//! \param play
//! \param parent
//!
KSoundChain::KSoundChain(bool play, QObject *parent) : KSoundArray(play, parent) {
    stepTimer.start();
    stepTimer.stop();
    connect(this, &KSoundArray::soundAdded, this, [this](Sound*) {
        if(sounds().size() == 1) {
            reset();
        }
    });
}

//!
//! \brief KSoundChain::reset
//!
void KSoundChain::reset() {
    if(!sounds().isEmpty()) {
        currentSound = sounds().first();
    }
}

//!
//! \brief KSoundChain::signal
//! \param index
//! \return
//!
double KSoundChain::signal(qint64 index) {
    if(play()) {
        stepTimer.resume();
    } else {
        stepTimer.stop();
    }


    if(stepTimer.elapsedAll() > interval()) {
        switchSound();
        stepTimer.restart();
    }

    if(currentSound) {
        return currentSound->volume(index);
    }
    return 0;
}
