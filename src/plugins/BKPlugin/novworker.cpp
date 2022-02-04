#include "novworker.h"

#include <QDataStream>
#include <QtDebug>
#include <tools/convert.h>
#include <cdebug.h>


NOVWorker::NOVWorker()
{

}

bool NOVWorker::proceedPacket(const QByteArray &ar, int msec){
    rxBuffer.push_back(ar);
}

void NOVWorker::send(QByteArray data)
{
    txBuffer.push_back(data);
}

bool NOVWorker::receive(QByteArray &data)
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

bool NOVWorker::receive(int &value){
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

QList<QByteArray> NOVWorker::getPacketsForSend(int msec){
    QList<QByteArray> buffer = txBuffer;
    txBuffer.clear();
    return buffer;
}

void NOVWorker::initNov(){
    cmd = NovPacketParser::debugGenerationSend(0, false);
    send(cmd);

    cmd = NovPacketParser::setDebugEnable(0, true, 0);
    send(cmd);

    cmd = NovPacketParser::nvo_par_send(0, 3, 1, 0, 0, 0);
    send(cmd);
}

void NOVWorker::startOnceBlink(){
    cmd = NovPacketParser::startOnceMeasure(0);
    send(cmd);
}
