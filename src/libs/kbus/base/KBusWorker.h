/*!
    \brief Base class for all KBus Workers
    \authors oleksii
    \version 0.1
    \date 26.04.2021
*/

#ifndef KBUSWORKER_H
#define KBUSWORKER_H

#include <QObject>
#include <QByteArray>

#include "KBus_global.h"

class KBUS_EXPORT KBusWorker : public QObject
{
    Q_OBJECT

protected:
    uint8_t id;

public:
    KBusWorker(QObject * parent = nullptr);
    virtual ~KBusWorker() {}

    virtual bool proceedPacket(const QByteArray &ar, int msec) = 0;
    virtual QList<QByteArray> getPacketsForSend(int msec) = 0;

    void setID(uint8_t id) { this->id = id; }
};

#endif // KBUSWORKER_H
