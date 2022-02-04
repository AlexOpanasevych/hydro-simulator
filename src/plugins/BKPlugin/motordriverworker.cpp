#include "motordriverworker.h"
#include <QDataStream>
#include <QtDebug>
#include "tools/convert.h"
#include "cdebug.h"

MotorDriverWorker::MotorDriverWorker()
{

}

bool MotorDriverWorker::proceedPacket(const QByteArray &ar, int msec){
    rxBuffer.push_back(ar);
}

void MotorDriverWorker::send(QByteArray data)
{
    txBuffer.push_back(data);
}

bool MotorDriverWorker::receive(QByteArray &data)
{
    if(!rxBuffer.empty()){
        data = rxBuffer.front();
        rxBuffer.pop_front();

        return true;
    }
    else{
        return false;
    }
}

bool MotorDriverWorker::receive(int &value){
    QByteArray data;

    if(receive(data)){
        QDataStream ds(data);
        ds.setByteOrder(QDataStream::LittleEndian);
        ds >> value;

        return true;
    }
    else{
        return false;
    }
}

QList<QByteArray> MotorDriverWorker::getPacketsForSend(int msec){
    QList<QByteArray> buffer = txBuffer;
    txBuffer.clear();
    return buffer;
}

void MotorDriverWorker::calculate_settings(double angleVelocity) {
    double FPGA_CLK = 100000000.0;
    double pulses_per_rotation = 200.0;
    double maxRotation = 200;

    if(angleVelocity == 0){
        driverParams.step = 0;
    }
    else{
        auto stp = abs(angleVelocity) / (360.0 / pulses_per_rotation);

        driverParams.speed = quint32(FPGA_CLK / (stp * 2.0));
        driverParams.step = (maxRotation / 360.) * pulses_per_rotation;
        driverParams.dir = angleVelocity < 0;
    }
}

void MotorDriverWorker::fixMotor(bool onFixed){
    driverParams.fix = onFixed;

    QByteArray cmd;

    cmd.append(quint8(0x0f)); // board id
    cmd.append(quint8(0x00));       // token id
    cmd.append(quint8(0x01));     // ma
    cmd.append(quint8(0x01));     // da
    cmd.append(driverParams.fix);     // data

    send(cmd);
}

void MotorDriverWorker::send_step_preset()
{
    QByteArray cmd;

    quint8 dir = quint8(quint8(driverParams.dir & quint8(0x01)) | (driverParams.fix << 1));

    cmd.append(quint8(0x0f)); // board id
    cmd.append(quint8(0x00));       // token id
    cmd.append(quint8(0x01));     // ma
    cmd.append(quint8(0x00));     // da


    cmd.append(quint8(driverParams.step));
    cmd.append(quint8(driverParams.step >> 8));
    cmd.append(quint8(driverParams.step >> 16));
    cmd.append(quint8(driverParams.step >> 24));

    cmd.append(quint8(driverParams.speed));
    cmd.append(quint8(driverParams.speed >> 8));
    cmd.append(quint8(driverParams.speed >> 16));
    cmd.append(quint8(driverParams.speed >> 24));

    cmd.append(dir);

    send(cmd);
}

void MotorDriverWorker::sendAngleVelocity(double angleVelocity){
    calculate_settings(angleVelocity);
    send_step_preset();
}

void MotorDriverWorker::fixPosition(){

}
