#include "statecommunicator.h"
#include <MessageType.h>
#include <QJsonDocument>
#include <QJsonObject>
#include "clientservice.h"

StateCommunicator::StateCommunicator(ClientService * service, QObject *parent) : QObject(parent)
{
    this->cService = service;
    connect(this, qOverload<QVariant, MGKItem*>(&StateCommunicator::requestValueChanged), this, [this](QVariant value, MGKItem * item){
        Frame frame;
        QDataStream s(&frame.data, QIODevice::WriteOnly);
        auto resource = item->resource();
        qDebug() << "request value changed, stateId" << resource->stateId()<< ", value:" << value;
        s << VALUE_CHANGED << resource->station() << resource->stateId() << resource->name() << cService->clientName() << value << '\0';
        cService->worker.writeData(frame);
    });
}

StateCommunicator::StateCommunicator(QObject *parent) : QObject(parent)
{
}

void StateCommunicator::receiveFromServer(int stateId, QVariant value)
{

    qDebug() << "received stateid: " << stateId << value;

    auto result = std::find_if(cService->itemsList.begin(), cService->itemsList.end(), [stateId](MGKItem * item){
        return item->resource()->stateId() == stateId;
    });

    if(result != cService->itemsList.end()) {
        (*result)->setValue(value);
    }
}

//void StateCommunicator::sendValueToServer(QVariant value)
//{


////    ResourceData data;
////    data.value = value;
////    data.stateId = sender->currentState()->stateId();
////    data.station = sender->currentState()->station();
////    data.action = sender->currentState()->action();
////    sendQueue.enqueue(data);

////    if(sender->currentState()->serializeType() == "bool") s <<  value.toBool();
////    if(sender->currentState()->serializeType() == "int") s << value.toInt();
////    if(sender->currentState()->serializeType() == "string") s << value.toString();
////    if(sender->currentState()->serializeType() == "double") s << value.toDouble();
//}
