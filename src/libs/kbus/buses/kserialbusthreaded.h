/*!
    \brief Implementation KBus on QSerialPort, QThread and Workers
    \authors oleksii, VladyslavN
    \version 0.1
    \date 26.04.2021
*/

#ifndef KSERIALBUSTHREADED_H
#define KSERIALBUSTHREADED_H

#include <QFuture>
#include <QSerialPort>
#include <QElapsedTimer>
#include <QSerialPortInfo>
#include <QtConcurrent/QtConcurrent>

#include "vdebug.h"
#include "base/KBus.h"
#include "coms/KRawParser.h"
#include "tools/circlebuf.h"

class KBUS_EXPORT KSerialBusThreaded: public KBus
{
private:
    KRawParser com;
    QThread *runThread;

    bool stopFlag = 0;
    QElapsedTimer timer;
    double stopWorkPos = -1;
    QFuture<void> m_runTask;

    const qint64 m_readBuffSize = 1024;
    CircleBuf<QByteArray> m_receiveBuf = CircleBuf<QByteArray>(1000);
    CircleBuf<QByteArray> m_transmitBuf = CircleBuf<QByteArray>(1000);

    void run();
    void logTime(const QByteArray &);
    void onPacketReceived(const QByteArray &);

    QSerialPort *serialPort = nullptr;
    std::function<void(int no, QByteArray data)> m_failCallback = nullptr;
    std::function<bool(const QByteArray&)> m_filterCallback = nullptr;

protected:
    void startOverride() override;

public:
    KSerialBusThreaded();
    ~KSerialBusThreaded() override;

    bool isEmptyTX()   override;
    bool isEmptyRX()   override;
    bool isConnected() override;

    void push(const QByteArray & data) override;
    QByteArray pop() override;

    void stopThread();

    bool setPort(QString name, uint64_t baudRate, QSerialPort::Parity parity);

    static QList<ConnectionData> getConnectionData();
};

#endif // KSERIALBUSTHREADED_H
