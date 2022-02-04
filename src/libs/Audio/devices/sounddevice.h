#ifndef KSOUNDDEVICE_H
#define KSOUNDDEVICE_H

#include <QAudioFormat>
#include <QIODevice>

//#include <src/lib/audio/ksound.h>
#include "sound.h"

#include <math/kmath.h>

//!
//! \brief The KSoundDevice class
//!
class AUDIO_EXPORT KSoundDevice : public QIODevice {
    Q_OBJECT
    K_AUTO_PROPERTY(Sound*, rootSound, rootSound, setRootSound, rootSoundChanged, nullptr)
public:
    //!
    //! \brief constructor for KSoundDevice class
    //! \param format
    //! \param durationUs
    //! \param sampleRate
    //! \param parent
    //!
    KSoundDevice(const QAudioFormat &format,
                 qint64 durationUs,
                 int sampleRate,
                 QObject *parent = nullptr);

    KSoundDevice(QObject * parent);
    KSoundDevice(Sound *device, QObject * parent);

    //!
    //! \brief Function for getting buffer size
    //! \return Returns buffer size
    //!
//    int bufferSize() {return m_buffer.size();}
private:
//    qint64 m_pos = 0;
//    QByteArray m_buffer;
//    std::function<void(const QAudioFormat &, qint64, int)> generatorCallback;
//    void generateData(const QAudioFormat &format, qint64 durationUs, int sampleRate);

    // QIODevice interface
public:
    //!
    //! \brief Function that overloads QIODevice::readData and used by QAudioOutput
    //! \param data
    //! \param maxlen
    //! \return Returns count of readed bytes
    //!
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 len) override;
//    qint64 bytesAvailable() const override;
};








#endif // KSOUNDDEVICE_H
