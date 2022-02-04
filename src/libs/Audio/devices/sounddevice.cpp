#include "sounddevice.h"
#include<QtMath>
#include <qendian.h>

KSoundDevice::KSoundDevice(const QAudioFormat &format
                           , qint64 durationUs
                           , int sampleRate
                           , QObject *parent) :
    QIODevice(parent) {
    Q_UNUSED(format)
    Q_UNUSED(durationUs)
    Q_UNUSED(sampleRate)
//    using namespace std::placeholders;
//    generatorCallback = std::bind(&KSoundDevice::generateData, this, _1, _2, _3);
//    if(format.isValid()) {
//        generatorCallback(format, durationUs, sampleRate);
//    }
}

KSoundDevice::KSoundDevice(QObject *parent) : QIODevice(parent)
{

}

KSoundDevice::KSoundDevice(Sound *device, QObject *parent) : QIODevice(parent)
{
    setRootSound(device);
}

//void KSoundDevice::start()
//{
//    open(QIODevice::ReadOnly);
//}

//void KSoundDevice::stop()
//{
//    m_pos = 0;
//    close();
//}

//void KSoundDevice::setGenerator(std::function<void (const QAudioFormat &, qint64, int)> callback)
//{
//    generatorCallback = callback;
//}

//QByteArray KSoundDevice::buffer()
//{
//    QByteArray buff = m_buffer;
//    m_buffer.clear();
//    return buff;
//}

//void KSoundDevice::generateData(const QAudioFormat &format, qint64 durationUs, int sampleRate)
//{
//    const int channelBytes = format.sampleSize() / 8;
//    const int sampleBytes = format.channelCount() * channelBytes;
//    qint64 length = (format.sampleRate() * format.channelCount() * (format.sampleSize() / 8))
//                        * durationUs / 1000000;
//    Q_ASSERT(length % sampleBytes == 0);
//    Q_UNUSED(sampleBytes) // suppress warning in release builds

//    m_buffer.resize(length);
//    unsigned char *ptr = reinterpret_cast<unsigned char *>(m_buffer.data());
//    int sampleIndex = 0;

//    while (length) {
//        // Produces value (-1..1)
//        const qreal x = qSin(2 * M_PI * sampleRate * qreal(sampleIndex++ % format.sampleRate()) / format.sampleRate());
//        for (int i=0; i<format.channelCount(); ++i) {
//            if (format.sampleSize() == 8) {
//                if (format.sampleType() == QAudioFormat::UnSignedInt) {
//                    const quint8 value = static_cast<quint8>((1.0 + x) / 2 * 255);
//                    *reinterpret_cast<quint8 *>(ptr) = value;
//                } else if (format.sampleType() == QAudioFormat::SignedInt) {
//                    const qint8 value = static_cast<qint8>(x * 127);
//                    *reinterpret_cast<qint8 *>(ptr) = value;
//                }
//            } else if (format.sampleSize() == 16) {
//                if (format.sampleType() == QAudioFormat::UnSignedInt) {
//                    quint16 value = static_cast<quint16>((1.0 + x) / 2 * 65535);
//                    if (format.byteOrder() == QAudioFormat::LittleEndian)
//                        qToLittleEndian<quint16>(value, ptr);
//                    else
//                        qToBigEndian<quint16>(value, ptr);
//                } else if (format.sampleType() == QAudioFormat::SignedInt) {
//                    qint16 value = static_cast<qint16>(x * 32767);
//                    if (format.byteOrder() == QAudioFormat::LittleEndian)
//                        qToLittleEndian<qint16>(value, ptr);
//                    else
//                        qToBigEndian<qint16>(value, ptr);
//                }
//            }

//            ptr += channelBytes;
//            length -= channelBytes;
//        }
//    }
//}

qint64 KSoundDevice::readData(char *data, qint64 maxlen) {
    const auto maxReadCount = 30000;
    const auto maxAmplitude = 10000;
//    const auto sampleRate = 48000;
    if (maxlen > maxReadCount) maxlen = maxReadCount;
    auto data16 = reinterpret_cast<qint16*>(data);
//    m_rootSound->setSampleRate(sampleRate);
    for (qint64  i = 0; i < maxlen / 2; ++i) {
        data16[i] = static_cast<qint16>(KMath::instance()->dmap(m_rootSound->volume(i), -m_rootSound->amplitude(), rootSound()->amplitude(), -maxAmplitude, maxAmplitude));
    }
//    m_buffer += data;
//    emit readyRead();
    return (maxlen / 2) * 2;
}

qint64 KSoundDevice::writeData(const char *data, qint64 len) {
    Q_UNUSED(data)
    Q_UNUSED(len)
    return 0;
}

//qint64 KSoundDevice::bytesAvailable() const
//{
//    return m_buffer.size() + QIODevice::bytesAvailable();
//}
