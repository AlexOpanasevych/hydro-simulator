#include "ksoundmix.h"

#include <math.h>
#include <algorithm>

//!
//! \brief KSoundMix::average
//! \return
//!
bool KSoundMix::average() const {
    return m_average;
}

//!
//! \brief KSoundMix::setAverage
//! \param average
//!
void KSoundMix::setAverage(bool average) {
    m_average = average;
}

//!
//! \brief KSoundMix::overrideAddSound
//! \param sound
//!
void KSoundMix::overrideAddSound(Sound *sound)
{
    sound->setAmplitude(this->amplitude());
    sound->setFrequency(this->frequency());
    sound->setSampleRate(this->sampleRate());
    sound->setPhase(this->phase());
    sound->setInterval(this->interval());
    sound->setPlay(this->play());
}

//!
//! \brief KSoundMix::KSoundMix
//! \param play
//! \param parent
//!
KSoundMix::KSoundMix(bool play, QObject *parent) : KSoundArray(play, parent) {

    connect(this, &Sound::frequencyChanged, this, [this](double value){
        for(auto && sound : sounds()) {
            sound->setFrequency(value);
        }
    });

    connect(this, &Sound::amplitudeChanged, this, [this](double value){
        for(auto && sound : sounds()) {
            sound->setAmplitude(value);
        }
    });

    connect(this, &Sound::sampleRateChanged, this, [this](double value){
        for(auto && sound : sounds()) {
            sound->setSampleRate(value);
        }
    });

    connect(this, &Sound::phaseChanged, this, [this](double value){
        for(auto && sound : sounds()) {
            sound->setPhase(value);
        }
    });

    connect(this, &Sound::intervalChanged, this, [this](double value){
        for(auto && sound : sounds()) {
            sound->setInterval(value);
        }
    });
}

//!
//! \brief KSoundMix::signal
//! \param index
//! \return
//!
double KSoundMix::signal(qint64 index) {
    double result = 0;
    double count = 0;
    for(Sound *sound : sounds()) {
        result += sound->volume(index);
        count++;
    }
    if(average() && !qFuzzyCompare(count, 0)) {
        result /= count;
    }
    //result = std::min(result, 1.0);
    //result = std::max(result, -1.0);
    return result;
}
