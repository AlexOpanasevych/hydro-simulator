#include "platina.h"

#include <src/abstractfactory.h>
#include <src/subsystems/mgkfsm.h>
#include <streamconnector.h>


Platina::Platina(QObject *parent) : Station("platina", parent)
{
    AbstractFactory::registerItem<MgkFSM>();
    addSubsystem(AbstractFactory::produceItem("MgkFSM"));

    StreamConnector::instance()->addStreamSender("epo1");
    StreamConnector::instance()->addStreamSender("position");


    AbstractFactory::registerItem<PlatinaTransmitter>();
    AbstractFactory::registerItem<PlatinaReceiver>();

    plTransmitter = dynamic_cast<PlatinaTransmitter*>(AbstractFactory::produceItem("PlatinaTransmitter"));
    addSubsystem(plTransmitter);

    plReceiver = dynamic_cast<PlatinaReceiver*>(AbstractFactory::produceItem("PlatinaReceiver"));
    addSubsystem(plReceiver);
}
