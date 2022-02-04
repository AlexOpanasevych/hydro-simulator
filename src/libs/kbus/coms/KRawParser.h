/*!
    \brief One of the implementations KCommunicator
    \authors oleksii
    \version 0.1
    \date 26.04.2021
    \details Very reliable and fast guarantee of data integrity and packet formation
*/

#ifndef QTRAWPARSER_H
#define QTRAWPARSER_H

#include <vector>
#include <QDebug>
#include <cinttypes>
#include <QByteArray>

#include "base/KCommunicator.h"

class KRawParser: public KCommunicator
{
private:
    const static uint8_t   startByte    = 0x1A;
    const static uint32_t  bufferSize   = 128;
    const static uint8_t   frameMaxSize = 32;

    uint32_t needProceedFrame = 0;

    uint8_t  m_buffer[bufferSize];
    uint8_t  m_frame[frameMaxSize];

    uint8_t m_frameLen = 0;
    uint8_t m_frameCrc = 0xFF;

    bool packReady    = false;
    bool m_lastWasSB  = false;
    bool m_receiveNow = false;

    uint32_t m_pos = 0;
    uint32_t writePos = 0;
    uint32_t readPos = 0;

    void _proceedUnshieldedDataStream(uint8_t ch, bool newFrame);
    static uint8_t _proceedCrc(uint8_t crc, uint8_t ch);

public:
    struct Frame {
        uint8_t *data;
        uint8_t size;
    };

    KRawParser();
    void proceedByte(uint8_t byte);
    QByteArray shieldFrame(QByteArray data);
    QByteArray readUnshieldedFrame();

    uint8_t getMaxFrameLength();

    template<typename T>
    void proceedArray(const T &array) { for(auto c : array) { proceedByte(c); } }

    template<typename T>
    void proceedArray(T array, size_t size) { for(size_t i = 0; i < size; ++i) { proceedByte(array[i]); } }

    void proceedRawData(const QByteArray &) override;
    void send(QByteArray &) override;

};

#endif // QTRAWPARSER_H
