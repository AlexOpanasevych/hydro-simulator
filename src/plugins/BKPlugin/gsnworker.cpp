#include <QDataStream>
#include <QtDebug>
#include "gsnworker.h"
#include "tools/convert.h"
#include "cdebug.h"


GSNWorker::GSNWorker(QObject * parent) : KBusWorker(parent)
{
    cmd.push_back(quint8(0x00));
    cmd.push_back(quint8(0x00));
    cmd.push_back(quint8(0x07));
    cmd.push_back(quint8(0x00));
    cmd.push_back(quint8(0x00));
}

bool GSNWorker::proceedPacket(const QByteArray &ar, int msec){
    rxBuffer.push_back(ar);
}

void GSNWorker::send(QByteArray data)
{
    txBuffer.push_back(data);
}

bool GSNWorker::receive(QByteArray &data)
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

bool GSNWorker::receive(int &value){
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


void GSNWorker::setLeftCirculation()
{
    Convert::setBit(reinterpret_cast<quint8*>(&cmd.data()[3]), 6, 0);
    Convert::setBit(reinterpret_cast<quint8*>(&cmd.data()[3]), 5, 1);
}

void GSNWorker::setRightCirculation()
{
    Convert::setBit(reinterpret_cast<quint8*>(&cmd.data()[3]), 5, 0);
    Convert::setBit(reinterpret_cast<quint8*>(&cmd.data()[3]), 6, 1);
//    qDebug() << "cmd[3]:" << QString::number(cmd[3], 2) << cmd.toHex();
}

void GSNWorker::setTopCirculation()
{
    Convert::setBit(reinterpret_cast<quint8*>(&cmd.data()[4]), 0, 0);
    Convert::setBit(reinterpret_cast<quint8*>(&cmd.data()[3]), 7, 1);
}

void GSNWorker::setBottomCirculation()
{
    Convert::setBit(reinterpret_cast<quint8*>(&cmd.data()[3]), 7, 0);
    Convert::setBit(reinterpret_cast<quint8*>(&cmd.data()[4]), 0, 1);
}

void GSNWorker::setNoCirculation()
{
    Convert::setBit(reinterpret_cast<quint8*>(&cmd.data()[3]), 5, 0);
    Convert::setBit(reinterpret_cast<quint8*>(&cmd.data()[3]), 6, 0);

    Convert::setBit(reinterpret_cast<quint8*>(&cmd.data()[3]), 0, 0);
    Convert::setBit(reinterpret_cast<quint8*>(&cmd.data()[3]), 1, 0);
    Convert::setBit(reinterpret_cast<quint8*>(&cmd.data()[3]), 2, 0);
}

void GSNWorker::setSpeed(int omegaInd)
{
    for(int i = 0; i < 3; i++){
        Convert::setBit(reinterpret_cast<quint8*>(&cmd.data()[3]), i, (i == omegaInd - 1));
    }
}

QByteArray GSNWorker::getCmd()
{
    return this->cmd;
}

QList<QByteArray> GSNWorker::getPacketsForSend(int msec){
    QList<QByteArray> buffer = txBuffer;
    txBuffer.clear();
    return buffer;
}

void GSNWorker::sendCmd(){
//    qDebug(k) << "sendCmd" << QString::number(cmd[3], 2);
    send(cmd);
}
