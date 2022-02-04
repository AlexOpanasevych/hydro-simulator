#ifndef STMWORKER_H
#define STMWORKER_H

#include "base/KBusWorker.h"
#include <QBitArray>
#include <QObject>

class GSNWorker : public KBusWorker
{
    Q_OBJECT
public:
    GSNWorker(QObject *parent = nullptr);
    ~GSNWorker(){};

    bool proceedPacket(const QByteArray &ar, int msec) override;
    QList<QByteArray> getPacketsForSend(int msec) override;

    void send(QByteArray data);

    bool receive(QByteArray &data);
    bool receive(int &value);

    void setLeftCirculation();
    void setRightCirculation();
    void setTopCirculation();
    void setBottomCirculation();
    void setNoCirculation();

    void setSpeed(int omegaInd);

    QByteArray getCmd();
    void sendCmd();
private:

    QList<QByteArray> rxBuffer;
    QList<QByteArray> txBuffer;

    QByteArray cmd;
    quint8 byte;

signals:
    void valueReceived(int value);
};

#endif // STMWORKER_H
