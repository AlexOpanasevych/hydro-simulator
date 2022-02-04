#include "positionunit.h"
#include <streamconnector.h>
#include <QtConcurrent/QtConcurrent>
#include <QNetworkDatagram>
#include <abstractengine.h>

PositionUnit::PositionUnit(QObject * parent) : AbstractSubsystem(parent)
{

    timer = new QTimer;
    connect(timer, &QTimer::timeout, this, [this](){
        for(auto & c : m_engine->getPiControllers()){
            auto info = c.second->getInfo();
            if(info["objectType"] == "Torpedo") {
                QByteArray data;
                data.append(info["circulation"].toInt());
                sock->writeDatagram(QNetworkDatagram(data, QHostAddress::Broadcast, 55555));

            }
        }
    });
    timer->start(200);

}

void PositionUnit::proceed(KObservableList<Resource> &stateList, const StateData &data)
{
    Q_UNUSED(data)
    Q_UNUSED(stateList)

//    for(auto && state : stateList) {

//    }
}

int PositionUnit::order()
{
    return 0;
}

PositionUnit::~PositionUnit() {
}

void PositionUnit::init()
{

}
