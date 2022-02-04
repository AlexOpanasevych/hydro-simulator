#include "src/abstractfactory.h"
#include "src/subsystems/mgkfsm.h"
#include "src/subsystems/positionunit.h"
#include "src/subsystems/receiver.h"
#include "src/subsystems/transmitter.h"
#include "station.h"
#include "serverservice.h"
#include <streamconnector.h>
#include <QThreadPool>
#include <cdebug.h>

//std::vector<Resource *> Station::statesList() const
//{
//    return m_statesList;
//}

//void Station::setStatesList(const std::vector<Resource*> &statesList)
//{
//    m_statesList = statesList;
//}

ServerService *Station::service() const
{
    return m_service;
}

void Station::setService(ServerService *service)
{
    m_service = service;
}

void Station::setEngine(AbstractEngine *engine)
{
    this->engine = engine;
    for(auto subsystem : m_subsysList) {
        subsystem->setEngine(engine);
    }
}

void Station::initializeResources()
{
    clearResources();
    dbProvider.setTable("QML");
    auto result = dbProvider.select({"id", "name", "controlName", "controlType"}).get();
    for(auto resource : result) {
        QString name = resource["name"].toString();
        if(m_name == name.chopped(1)) {
            Resource * res = new Resource;
            res->setStn(this);
            res->setStateId(resource["id"].toInt());
            res->setStation(m_name);
            res->setClient(name);
            res->setName(resource["controlName"].toString());
            res->setControlType(resource["controlType"].toString());
            m_statesList.push_back(res);
        }
    }

    std::sort(m_statesList.begin(), m_statesList.end(), [](const Resource * left, const Resource * right){
        return left->stateId() < right->stateId();
    });

}

QString Station::name() const
{
    return m_name;
}

void Station::setName(const QString &name)
{
    m_name = name;
}

QVariantList Station::getStateDataAsMap(const QString & clientName)
{
    QVariantList resultList;
    for(auto res : m_statesList) {
        if(res->client() == clientName) {
            QVariantMap map;
            map["stateId"] = res->stateId();
            map["value"] = res->value();
            resultList.append(map);
        }
    }
    return resultList;
}


Resource * Station::getStateByName(const QString &name, bool *ok)
{
    auto result
            = std::find_if(m_statesList.begin(), m_statesList.end(), [name](const Resource * state){
        return state->name() == name;
    });
    if(result == m_statesList.end()) *ok = false;
    return *result;
}

Resource *Station::getStateById(int stateId, bool *ok)
{
    QList<Resource*>::iterator res;
    *ok = (res = std::find_if(m_statesList.begin(), m_statesList.end(), [&stateId](const Resource * res) {

        return stateId == res->stateId();
    })) != m_statesList.end();
    return *res;
}

void Station::setSendCallback(const std::function<void (const QList<StateData> &)> &sendCallback)
{
    m_sendCallback = sendCallback;
}

void Station::sendChangesList(const QList<StateData> & list)
{
    QMutexLocker mLocker(&m);
    m_sendCallback(list);
}

void Station::stop()
{
    emit stopAllSubsystems();
}

void Station::onResourceChanged(const StateData & data)
{
    QByteArray name = data.resourceName.toUtf8();
    auto search = m_methods.find(name);
    if (search != m_methods.end()) /*{
        for (int  i = metaObject()->methodOffset(); i < metaObject()->methodCount(); i++) {
            auto m = metaObject()->method(i);
            if (0 == m.name().compare(name, Qt::CaseInsensitive)) {
                search = m_methods[name].append({});
                call = m;
            }
        }
    } else */{
        for(auto & call : search.value()) {
            call.m.invoke(call.subsystem, Qt::DirectConnection, Q_ARG(StateData, data));
        }
    }
}

ServerService *Station::getService() const
{
    return m_service;
}

void Station::addmethods(AbstractSubsystem *sys)
{
    for(auto & state : m_statesList) {

        for (int  i = sys->metaObject()->methodOffset(); i < sys->metaObject()->methodCount(); i++) {
            auto m = sys->metaObject()->method(i);
            auto name = state->name().toLower();
            auto methodName = m.name().toLower();
//            name = name[0].toUpper() + name.mid(1);
            if (methodName.contains("on" + name.toUtf8()) && !name.isEmpty()) {
                m_methods[state->name()].append(Call {.subsystem = sys, .m = m});
            }
        }
    }
}

//void Station::proceedState(const StateData & data)
//{
////    auto result = globalProceed(data);
////    if(!result.empty() and m_sendCallback) m_sendCallback(result);
//}

Station::Station(QString name, QObject *parent) : QObject(parent), m_name(name), dbProvider("../../../src/plugins/ServerInterface/QML.db")
{

    m_stateModel = m_statesList.model();
    m_stateModel->setParent(this);
    initializeResources();

    AbstractFactory::registerItem<PositionUnit>();
    addSubsystem(AbstractFactory::produceItem("PositionUnit"));
    qRegisterMetaType<StateData>("StateData");

}

Station::~Station()
{
    qDebug(s) << "deleting fucking station";
    stop();

    //clear lists
    qDeleteAll(m_subsysList);
    qDeleteAll(m_statesList.begin(), m_statesList.end());
}

std::vector<AbstractSubsystem *> Station::subsysList() const
{
    return m_subsysList;
}

void Station::addSubsystem(AbstractSubsystem *subsystem) {
    subsystem->setStation(this);
    subsystem->init();
    addmethods(subsystem);
    m_subsysList.push_back(subsystem);

    connect(this, &Station::pushState, subsystem, [this, subsystem](const StateData & data){
        subsystem->proceed(m_statesList, data);
    }, Qt::DirectConnection);

    connect(this, &Station::stopAllSubsystems, subsystem, &AbstractSubsystem::deleteLater);
}
