#include "sounddevice.h"
#include<QtMath>
#include <qendian.h>

KClientSoundDevice::KClientSoundDevice(QObject *parent) : QIODevice(parent) {}

void KClientSoundDevice::start()
{
    open(QIODevice::ReadWrite);
}

void KClientSoundDevice::stop()
{
    m_pos = 0;
    close();
}


QByteArray &KClientSoundDevice::buffer()
{
    return m_buffer;
}

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

qint64 KClientSoundDevice::readData(char *data, qint64 maxlen) {
    qint64 total = 0;
    if (maxlen > maxReadCount) maxlen = maxReadCount;
    if(!m_buffer.isEmpty()) {
        while (maxlen - total > 0) {
            const qint64 chunk = qMin((m_buffer.size() - m_pos), maxlen - total);
            memcpy(data + total, m_buffer.constData() + m_pos, chunk);
            m_pos = (m_pos + chunk) % m_buffer.size();
            total += chunk;
            m_buffer = m_buffer.mid(m_pos);
        }
    }
//    auto data16 = reinterpret_cast<qint16*>(data);
//    for (qint64  i = 0; i < maxlen / 2; ++i) {
//        data16[i] = static_cast<qint16>(KMath::instance()->dmap(m_rootSound->volume(i), -1, 1, -maxAmplitude, maxAmplitude));
//    }
    return total;
}

qint64 KClientSoundDevice::writeData(const char *data, qint64 len) {
    Q_UNUSED(len)
    m_buffer += data;
    emit readyRead();
    return 0;
}

qint64 KClientSoundDevice::bytesAvailable() const
{
    return m_buffer.size() + QIODevice::bytesAvailable();
}

//qint64 KSoundDevice::bytesAvailable() const
//{
//    return m_buffer.size() + QIODevice::bytesAvailable();
//}
