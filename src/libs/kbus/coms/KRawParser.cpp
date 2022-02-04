#include "KRawParser.h"

KRawParser::KRawParser()
{
    needProceedFrame = 0;
}

/**
 *	@brief Get data and shielded it
 *	@param array - data
 *	@retval shielded data
 */
QByteArray KRawParser::shieldFrame(QByteArray data)
{
    if (data.size() > frameMaxSize) {
        qDebug() << "[ ERROR ] Data to shield more than frameMaxSize";
        return QByteArray();
    }

    char crc = static_cast<char>(0xFF);
    QByteArray raw;

    raw.push_back(startByte);

    data.insert(0, static_cast<uint8_t>(data.size() >= 26 ? data.size() + 1 : data.size()));

    auto addByte = [&raw, &crc](uint8_t b) {
        crc = _proceedCrc(crc, b);
        raw.push_back(b);

        if (b == startByte) {
            raw.push_back(b);
        }
    };

    for (uint8_t i = 0, l = data.size(); i < l; i++) {
        addByte(data[i]);
    }

    addByte(crc);
    return raw;
}

void KRawParser::proceedByte(uint8_t byte)
{
    ++needProceedFrame;
    m_buffer[writePos] = byte;
    writePos = (writePos + 1) % bufferSize;

}

QByteArray KRawParser::readUnshieldedFrame()
{
    if (needProceedFrame <= 0)
        return QByteArray();

    --needProceedFrame;

    while (readPos != writePos)
    {
        uint8_t ch = m_buffer[readPos];
        readPos = (readPos + 1) % bufferSize;

        if (m_lastWasSB) // newframe
        {
            if(ch == startByte)
            {
                //{SB}{SB} -> {SB}
                _proceedUnshieldedDataStream(ch, false);
            }
            else
            {
                 //{SB}{!SB} -> {SB} and newframe
                _proceedUnshieldedDataStream(ch, true);
            }

            m_lastWasSB = false;
        }
        else if (ch == startByte)
        {
             //{!SB}{SB} -> set flag and skip step
            m_lastWasSB = true;
        }
        else
        {
             //{!SB}{!SB} -> {!SB}
            _proceedUnshieldedDataStream(ch, false);
        }

        if (packReady)
        {
            packReady = false;
            return QByteArray((const char*)m_frame, m_frameLen);
        }
    }

    return QByteArray();
}

uint8_t KRawParser::getMaxFrameLength()
{
    return frameMaxSize * 0.8;
}

void KRawParser::_proceedUnshieldedDataStream(uint8_t ch, bool newFrame)
{
    if (newFrame)
    {
        m_frameCrc = 0xFF;
        m_receiveNow = true;
        m_pos = 0;
    }

    if (!m_receiveNow)
        return;

    if (m_pos == 0)
    {
        m_frameLen = ch;

        if (m_frameLen > startByte)
            m_frameLen -= 1;
    }
    else if ((m_pos - 1) < m_frameLen) {
        m_frame[m_pos-1] = ch;
    } else if ((m_pos - 1) == m_frameLen && m_frameCrc == ch) {
            packReady = true;
    } else {
        return;
    }

    m_pos++;
    m_frameCrc = _proceedCrc(m_frameCrc, ch);
}

uint8_t KRawParser::_proceedCrc(uint8_t crc, uint8_t ch) {
    crc ^= ch;
    for (int i = 0; i < 8; i++)
        crc = crc & 0x80 ? (crc << 1) ^ 0x31 : crc << 1;
    return crc;
}

void KRawParser::proceedRawData(const QByteArray &data)
{
    proceedArray(data);
    QByteArray readedData = readUnshieldedFrame();
    if (readedData.size() > 0) {
        if (receiveCallback != nullptr) {
            receiveCallback(readedData);
        } else {
            VDebug::Warning("KRawParserCommunicator.receiveCallback == nullptr");
        }
    }
}

void KRawParser::send(QByteArray &data)
{
    qDebug() <<"SEND: " << data.toHex('.');
    QByteArray shieldData = shieldFrame(data);
    if (transmitCallback != nullptr) {
        transmitCallback(shieldData);
        return;
    }

    VDebug::Warning("KRawParserCommunicator.transmitCallback == nullptr");
}
