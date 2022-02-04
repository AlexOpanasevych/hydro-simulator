#include "bronzereceiver.h"
#include <src/station.h>

BronzeReceiver::BronzeReceiver()
{
    connect(timer(), &QTimer::timeout, this, &BronzeReceiver::receiverProceed);
}

void BronzeReceiver::proceed(KObservableList<Resource> &stateList, const StateData &data)
{
    QList<StateData> resultList;
    for(auto && state : stateList) {

    }
    if(!resultList.isEmpty()) station()->sendChangesList(resultList);
}

void BronzeReceiver::receiverProceed()
{

}

void BronzeReceiver::init()
{

}
