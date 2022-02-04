#ifndef AUDIO_H
#define AUDIO_H

#include "devices/sounddevice.h"

#include <QObject>
#include <QTimer>
#include <QAudioDeviceInfo>
#include <QAudioOutput>
#include <Audio_global.h>
#include <kmacro.h>
#include <audiostreaminglibcore.h>
#ifdef Q_OS_WIN
#include "utility/qwinloopback.h"
#endif

//!
//! \brief The Audio class
//!
class AUDIO_EXPORT Audio : public QObject
{
    Q_OBJECT
    K_QML_TYPE(Audio)
//    K_AUTO_PROPERTY(double, frequency, frequency, setFrequency, frequencyChanged, 500)
//    K_AUTO_PROPERTY(double, amplitude, amplitude, setAmplitude, amplitudeChanged, 0.5)
//    K_AUTO_PROPERTY(double, distance, distance, setDistance, distanceChanged, 1000)
//    K_AUTO_PROPERTY(double, delay, delay, setDelay, delayChanged, 0.5)
//    K_AUTO_PROPERTY(double, period, period, setPeriod, periodChanged, 1 / frequency())
//    K_AUTO_PROPERTY(double, bearing, bearing, setBearing, bearingChanged, 0.5)
    K_AUTO_PROPERTY(double, volume, volume, setVolume, volumeChanged, 0.5)
//    K_AUTO_PROPERTY(Sound*, rootSound, rootSound, setRootSound, rootSoundChanged, nullptr);

    quint64 timeFromStart = 0;
    QTimer * pushTimer = nullptr;
public:
    explicit Audio(QObject *parent = nullptr);
    void initializeAudio(const QAudioDeviceInfo & deviceInfo);
    void toggleSuspendResume();
    KSoundDevice *device();
//    QScopedPointer<QAudioOutput> &output();
    void changeVolume(double value);
//    int volume();
    ~Audio();
    QAudioOutput *audioOutput() const;
    void addSound(int id, QString name, QVariantMap params);
    void stopSound(int id, QString name = "");
private:
    KSoundDevice * audioDevice = nullptr;
    QAudioOutput * m_audioOutput = nullptr;

    QPointer<AudioStreamingLibCore> m_audio_lib;
    QByteArray m_buffer;
    QTimer playTimer;

#ifdef Q_OS_WIN
    QPointer<QWinLoopback> m_loopback;
#endif
};

#endif // AUDIO_H
