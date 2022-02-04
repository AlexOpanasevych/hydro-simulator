#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include "base/KBusWorker.h"
#include <QObject>


class MotorDriverWorker: public KBusWorker
{
public:
    MotorDriverWorker();

    bool proceedPacket(const QByteArray &ar, int msec) override;
    QList<QByteArray> getPacketsForSend(int msec) override;

    void send(QByteArray data);

    bool receive(QByteArray &data);
    bool receive(int &value);

    void fixMotor(bool onFixed);
    void sendAngleVelocity(double angleVelocity);
    void fixPosition();

private:

    QList<QByteArray> rxBuffer;
    QList<QByteArray> txBuffer;

    struct {
        quint32 step = 0;
        quint32 speed = 1;

        bool dir = true;
        bool fix = false;

    } driverParams;

    void calculate_settings(double angleVelocity);
    void send_step_preset();
signals:
    void valueReceived(int value);
};

#endif // MOTORDRIVER_H
