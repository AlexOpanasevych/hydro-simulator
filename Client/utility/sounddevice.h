#ifndef KSOUNDDEVICE_H
#define KSOUNDDEVICE_H

#include <QAudioFormat>
#include <QIODevice>

//#include <src/lib/audio/ksound.h>

#include <math/kmath.h>

class KClientSoundDevice : public QIODevice {
    Q_OBJECT
//    K_AUTO_PROPERTY(Sound*, rootSound, rootSound, setRootSound, rootSoundChanged, nullptr)
public:
//    KSoundDevice(const QAudioFormat &format,
//                 qint64 durationUs,
//                 int sampleRate,
//                 QObject *parent = nullptr);

    KClientSoundDevice(QObject * parent = nullptr);
//    KClientSoundDevice(Sound *device, QObject * parent);

    void start();
    void stop();

//    void setGenerator(std::function<void(const QAudioFormat &, qint64, int)>);
    QByteArray & buffer();
private:
    qint64 m_pos = 0;
    QByteArray m_buffer;
    const int maxReadCount = 30000;
    const int maxAmplitude = 10000;
    const int sampleRate = 48000;

    // QIODevice interface
public:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;
    qint64 bytesAvailable() const override;
};








#endif // KSOUNDDEVICE_H
