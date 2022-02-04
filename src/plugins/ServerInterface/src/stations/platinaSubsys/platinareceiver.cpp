#include "platinareceiver.h"
#include <utility/kslotregisterer.h>
#include "../../station.h"
#include <cdebug.h>
#include <math.h>
#include <streamconnector.h>

PlatinaReceiver::PlatinaReceiver(QObject *parent) : Receiver(parent)
{

    qRegisterMetaType<QVector<double>>();
    qRegisterMetaTypeStreamOperators<QVector<double>>();


    connect(timer(), &QTimer::timeout, this, &PlatinaReceiver::receiverProceed);
    timer()->start(5);


    connect(this, &PlatinaReceiver::scaleChanged, this, &PlatinaReceiver::changeMaxTime);

}


void PlatinaReceiver::proceed(KObservableList<Resource> &stateList, const StateData &data)
{
    Q_UNUSED(stateList)
    station()->onResourceChanged(data);
}

void PlatinaReceiver::receiverProceed()
{
//    switch(m_platinaMode) {
//    case PlatinaMode::O1: {


//        break;
//    }
//    case PlatinaMode::O2: {

//        break;
//    }
//    case PlatinaMode::C1: {

//        break;
//    }
//    case PlatinaMode::C2: {

//        break;
//    }

//    }
    static double time = 0;

    QVector<double> pressures(72);
    for(auto & pr : pressures) {
        pr = /*QRandomGenerator::global()->generateDouble() / 2 + */0.5;
    }

    time += 0.005;

    auto timePeriod = fmod(time, maxTime);

    QVariantMap m;
    m["time"] = timePeriod;
    m["pressures"] = QVariant::fromValue<QVector<double>>(pressures);

    StreamConnector::instance()->sendData(m, "epo1");
}

void PlatinaReceiver::search()
{
    switch (m_searchMode) {
    case SearchMode::FarNearSearch: {

        switch (m_searchSide) {
        case Side::Left: {



            break;
        }
        case Side::Right: {


            break;
        }
        }

        break;
    }
    case SearchMode::FarSearch: {

        switch (m_searchSide) {
        case Side::Left: {



            break;
        }
        case Side::Right: {


            break;
        }
        }

        break;
    }
    case SearchMode::NearSearch: {

        switch (m_searchSide) {
        case Side::Left: {



            break;
        }
        case Side::Right: {


            break;
        }
        }


        break;
    }
    }
}

void PlatinaReceiver::init()
{
}

void PlatinaReceiver::onEpChanged(const StateData & data) {
    if(data.value.canConvert<QVariantMap>()) {
        auto scData = data.value.toMap();
//        setScale(static_cast<Scale>(scData["index"].toInt()));
    }
    else {
        if(data.value.toInt() == 0) {
            setPlatinaMode(PlatinaMode::Shp);
        }
    }
}

void PlatinaReceiver::onScaleChanged(const StateData &data)
{
    if(data.value.canConvert<QVariantMap>()) {
        auto scData = data.value.toMap();
        setScale(static_cast<Scale>(scData["index"].toInt()));
    }
    else {
        setScale(static_cast<Scale>(data.value.toInt()));
    }
}

void PlatinaReceiver::onDpbpChanged(const StateData &data)
{
    if(data.value.canConvert<QVariantMap>()) {
        auto scData = data.value.toMap();
//        setScale(static_cast<Scale>(scData["index"].toInt()));
    }
    else {
        setSearchMode(static_cast<SearchMode>(data.value.toInt()));
    }
    qDebug() << "searchMode changed" << static_cast<int>(m_searchMode);
}

void PlatinaReceiver::changeMaxTime()
{
    changeMaxTimePrivate(static_cast<int>(m_scale) + 1);
}

void PlatinaReceiver::changeMaxTimePrivate(int times)
{

    maxTime = distanceMultiplier * std::pow(2, times);
    qDebug(s) << "changed time" << maxTime;
}

void PlatinaReceiver::proceedPlatinaModeChange()
{
    switch(m_platinaMode){
    case PlatinaMode::O1:
        switch (m_searchMode) {
        case SearchMode::FarNearSearch:
            [[fallthrough]];
        case SearchMode::FarSearch:
            changeMaxTime();
            break;
        case SearchMode::NearSearch:
            changeMaxTimePrivate(2);
            break;
        }
        break;
    case PlatinaMode::O2:
        changeMaxTimePrivate(2);
        break;
    case PlatinaMode::S1:
        [[fallthrough]];
    case PlatinaMode::S2:
        changeMaxTimePrivate(2);
        break;
    case PlatinaMode::Shp:
        break;
    }
}

void PlatinaReceiver::onO1o2Changed(const StateData & data)
{
    qDebug(s) << "o1o2 property changed" << data.value;

    if(data.value.canConvert<QVariantMap>()) {
        auto scData = data.value.toMap();
        setPlatinaMode(static_cast<PlatinaMode>(scData["index"].toInt()));
    }
    else {
        setPlatinaMode(static_cast<PlatinaMode>(data.value.toInt()));
    }
    proceedPlatinaModeChange();
}
