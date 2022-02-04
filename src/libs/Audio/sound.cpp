#include "sound.h"

int Sound::countObj = 0;

Sound::Sound(bool play, QObject *parent) : QObject(parent)
{
    setPlay(play);
    qDebug() << "sound created, count:" << countObj++;
}

//!
//! \brief Sound::init
//! \param params
//!
void Sound::init(QVariantMap params)
{
    setPlay(true);
    if(params["amplitude"].isValid()) setAmplitude(params["amplitude"].toDouble());
    if(params["frequency"].isValid()) setFrequency(params["frequency"].toDouble());
    if(params["phase"].isValid()) setPhase(params["phase"].toDouble());
    if(params["interval"].isValid()) setInterval(params["interval"].toDouble());
    if(params["sampleRate"].isValid()) setSampleRate(params["sampleRate"].toDouble());
}

//!
//! \brief Multiplies generated sound block by amplitude and returns it if play is enabled
//! \param index
//! \return volume
//!
double Sound::volume(qint64 index) {
    if(m_play)
        return signal(index) * amplitude()/* * explicitVolume()*/;
    return 0;
}
