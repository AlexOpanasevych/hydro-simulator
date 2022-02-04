#ifndef SOUND_H
#define SOUND_H

#include "Audio_global.h"

#include <QObject>
#include <kmacro.h>

#include <utility/kintegrator.h>

//!
//! \brief The Sound class
//!
class AUDIO_EXPORT Sound : public QObject
{
    static int countObj;
    Q_OBJECT
    K_QML_INTERFACE(Sound)
    K_AUTO_PROPERTY(bool, play, play, setPlay, playChanged, false)
    K_AUTO_PROPERTY(double, amplitude, amplitude, setAmplitude, amplitudeChanged, 1.0)
//    K_AUTO_PROPERTY(double, frequency, frequency, setFrequency, frequencyChanged, 0)
//    K_AUTO_PROPERTY(double, phase, phase, setPhase, phaseChanged, 0)
//    K_AUTO_PROPERTY(quint64, time, time, setTime, timeChanged, 0)
//    K_AUTO_PROPERTY(double, deltaTime, deltaTime, setDeltaTime, deltaTimeChanged, 0)
    K_AUTO_PROPERTY(double, sampleRate, sampleRate, setSampleRate, sampleRateChanged, 48000)
    K_AUTO_PROPERTY(double, frequency, frequency, setFrequency, frequencyChanged, 2000)
    K_AUTO_PROPERTY(double, phase, phase, setPhase, phaseChanged, 0)
    K_AUTO_PROPERTY(double, interval, interval, setInterval, intervalChanged, 0)
//    K_AUTO_PROPERTY(double, explicitVolume, explicitVolume, setExplicitVolume, explicitVolumeChanged, 1);
public:
    explicit Sound(bool play = true, QObject * parent = nullptr);

protected:
    KIntegrator integrator;
    virtual double signal(qint64 index) = 0;
public:
    virtual void init(QVariantMap params);
    double volume(qint64 index);
    virtual ~Sound() {
        qDebug() << "deleting, count:" << countObj--;
    }
};

#endif // SOUND_H
