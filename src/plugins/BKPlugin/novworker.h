#ifndef NOVWORKER_H
#define NOVWORKER_H

#include "nov_helper/nov_packet_parser.h"
#include <kbusfactory.h>


class NOVWorker : public KBusWorker
{
    Q_OBJECT
public:
    NOVWorker();
    ~NOVWorker(){};

    bool proceedPacket(const QByteArray &ar, int msec) override;
    QList<QByteArray> getPacketsForSend(int msec) override;

    void send(QByteArray data);

    bool receive(QByteArray &data);
    bool receive(int &value);

    void initNov();
    void startOnceBlink();
private:

    QList<QByteArray> rxBuffer;
    QList<QByteArray> txBuffer;

    NovPacketParser * novPacketParser = nullptr;

    QByteArray cmd;
};

#endif // NOVWORKER_H
