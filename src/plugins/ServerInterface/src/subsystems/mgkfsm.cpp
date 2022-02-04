#include "mgkfsm.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDir>
#include <src/station.h>
#include <cdebug.h>

MgkFSM::MgkFSM(QObject *parent) : AbstractSubsystem(parent)
{
    QFile file("../../../src/plugins/ServerInterface/stateTransitions.json");
    qDebug() << QDir().absolutePath();
    file.open(QIODevice::ReadOnly|QIODevice::Text);
    QByteArray data = file.readAll();
    file.close();

    QJsonParseError errorPtr;
    QJsonDocument doc = QJsonDocument::fromJson(data, &errorPtr);
    if (doc.isNull()) {
        qDebug() << "Parse failed" << errorPtr.error;
    }
    auto stateTransitions = doc.object();
    QJsonArray result = stateTransitions.value("states").toArray();
    qDebug() << result;
    map.setStateMachine(this);
    map.init(result);
}

void MgkFSM::proceed(KObservableList<Resource> &stateList, const StateData & data)
{
    auto result = std::find_if(stateList.begin(), stateList.end(), [data](const Resource * res){
        return res->stateId() == data.stateID;
    });

    if(result != stateList.end()) {

        QList<StateData> changesList;
        std::map<int, ChangesMap> extraValues;
        QVariantMap map;
        if(data.value.canConvert<QVariantMap>()) {
            map = data.value.toMap();
            auto extra = map["extra"];
            if(extra.isValid()) {
                if(extra.canConvert<QVariantList>()) {
                    extraValues = this->map.states[data.stateID].getByExtra(extra.toList());
                }
                else extraValues = this->map.states[data.stateID].getByExtra(extra);
            }
        }
        map.remove("extra");
        for(auto && extraState : extraValues) {
            if(extraState.second.hasExtra()) {
                if(extraState.second.extra.contains("any") && extraState.second.extra.contains("same")) {
                    auto result = std::find_if(stateList.begin(), stateList.end(), [&extraState](const Resource * res){
                        return res->stateId() == extraState.first;
                    });
                    StateData stData;
                    stData.stateID = extraState.first;
                    stData.socketDescriptor = data.socketDescriptor;
                    auto value = (*result)->value();
                    if(!extraState.second.valueChanges.empty()) {
                        QVariantMap mask;
                        QVariantMap changes;
                        if(value.canConvert<QVariantMap>()) {
                            mask = value.toMap();
                        }
                        for(auto change = extraState.second.valueChanges.begin(); change != extraState.second.valueChanges.end(); change++) {
                            if(change.value() != "same") {
                                mask[change.key()] = change.value();
                                changes[change.key()] = change.value();
                            }
                            else {
                                mask[change.key()] = map["value"];
                                changes[change.key()] = map["value"];
                            }
                        }

                        (*result)->setValue(mask);
//                        emit station()->propertyChanged((*result)->name(), (*result)->value());
                        stData.value = changes;
                    }
                    else {
                        if(value.canConvert<QVariantMap>() && data.value.canConvert<QVariantMap>()) {
                            auto conv = value.toMap();
                            auto dataValue = data.value.toMap();
                            for(auto && it = dataValue.begin(); it != dataValue.end(); it++) {
                                conv[it.key()] = it.value();
                            }
                            (*result)->setValue(conv);
//                            emit station()->propertyChanged((*result)->name(), (*result)->value());
                        }
                        else {
                            (*result)->setValue(data.value);
//                            emit station()->propertyChanged((*result)->name(), (*result)->value());

                        }
                        stData.value = data.value;
                    }
                    qDebug() << stData;
                    changesList.append(stData);
                }
            }
        }
        auto stateChanges = this->map.states[data.stateID].getByValue(data.value);


        for(auto && state : stateChanges) {
            auto result = std::find_if(stateList.begin(), stateList.end(), [this, state](const Resource * res){
                if(res->stateId() == state.first)
                    return this->map.checkForConditions(state.second.execValues);
                else return false;
            });
            if(result != stateList.end()) {
                StateData stData;
                stData.stateID = state.first;
                stData.socketDescriptor = data.socketDescriptor;
                auto value = (*result)->value();
                if(value.canConvert<QVariantMap>()) {
                    auto conv = value.toMap();
                    for(auto && it = state.second.valueChanges.begin(); it != state.second.valueChanges.end(); it++)
                        conv[it.key()] = it.value();
                    (*result)->setValue(conv);
                    qDebug(s) << (*result)->name() << (*result)->value();
//                    emit station()->propertyChanged((*result)->name(), (*result)->value());
                }
                else {
                    (*result)->setValue(state.second.valueChanges);
                    qDebug(s) << (*result)->name() << (*result)->value();
//                    emit station()->propertyChanged((*result)->name(), (*result)->value());
                }
                stData.value = state.second.valueChanges;
                changesList.append(stData);
            }
        }

        station()->sendChangesList(changesList);

    }
}

void MgkFSM::init()
{

}


bool TransitionMap::checkForConditions(const QVariantMap &conditions){

    if(conditions.isEmpty()) return true;

    bool result = true;

    for(auto it = conditions.constBegin(); it != conditions.constEnd(); it++) {
        QStringList nameProp = it.key().split('.');
        auto stateEntity = getByName(nameProp[0], &result);
        auto state = m_stateMachine->station()->getStateById(stateEntity.stateId, &result);
        if(result) {
            if(state->value().canConvert<QVariantMap>()) {
                auto map = state->value().toMap();
                auto value = it.value();
                auto resValue = map[nameProp[1]];
                if(value.canConvert<QVariantList>()) { // here we check if there are few values with same name
                    auto vList = value.toList();
                    result = (std::find_if(vList.begin(), vList.end(), [&resValue](const QVariant & val){
                        return val == resValue;
                    }) != vList.end());
                }
                else result = resValue == it.value();
                if(!result) {
                    qDebug() << "prop" << "(" << nameProp[0] << "," << it.value() << ")" << "!=" << "(" << nameProp[0] << map[nameProp[1]] << ")";
                    break;
                }
            }
            else result = false;
        }
        else {
            qDebug() << "state with desired name" << nameProp[0] << "not found";
            break;
        }
    }
    return result;
}

int MgkFSM::order()
{
    return 0;
}

void TransitionMap::init(const QJsonArray &entityData)
{
    for (auto && entity : entityData) {
        QJsonObject entityDataObj = entity.toObject();
        State state;
        state.stateId = entityDataObj.value("stateId").toInt();
        state.name = entityDataObj.value("name").toString();
        auto changesArray = entityDataObj.value("changes").toArray();
        for(auto && changes : changesArray) {
            ChangesMap changesMap;
            QJsonObject stateData = changes.toObject();

            changesMap.stateId = stateData.value("stateId").toInt();

            changesMap.value = stateData.value("value").toVariant();
            if(stateData.contains("extra")) {
                auto extraArray = stateData.value("extra").toArray();
                for(auto val : extraArray) {
                    changesMap.extra << val.toVariant();
                }
            }

            auto values = stateData.value("values").toObject();

            for(auto && value : values.keys()) {
                changesMap.valueChanges[value] = values.value(value).toVariant();
            }

            auto execValues = stateData.value("execValues").toObject();

            for(auto && value : execValues.keys()) {
                changesMap.execValues[value] = execValues.value(value).toVariant();
            }

            state.changes[changesMap.stateId].push_back(changesMap);
            qDebug() << changesMap.valueChanges;
        }

        states[state.stateId] = state;
    }
}

State TransitionMap::getByName(const QString &stateName, bool * ok)
{
    auto result = std::find_if(states.begin(), states.end(), [stateName](const std::pair<int, State> & state){
        return state.second.name == stateName;
    });
    if(result == states.end()) {
        *ok = false;
        return {};
    };
    return result->second;
}

TransitionMap::TransitionMap()
{
//    QObject::connect(m_stateMachine, &AbstractSubsystem::stationChanged, m_stateMachine, [this](){
//        for(auto it = states.begin(); it != states.end(); it++) {
//            bool ok;
//            auto res = m_stateMachine->station()->getStateById(it->second.stateId, &ok);
//            if(ok) res->setName(it->second.name);
//        }
//    });
}

void TransitionMap::setStateMachine(MgkFSM *stateMachine)
{
    m_stateMachine = stateMachine;
}

std::map<int, ChangesMap> State::getByValue(QVariant value)
{
    std::map<int, ChangesMap> matches;
    for(auto && change : changes) {

        for(auto && item : change.second) {
            if(item.value == value)
                matches[change.first] = item;
        }

    }
    return matches;
}

std::map<int, ChangesMap> State::getByExtra(QVariant extra)
{
    std::map<int, ChangesMap> matches;
    for(auto && change : changes) {

        for(auto && item : change.second) {
            if(item.extra.contains(extra))
                matches[change.first] = item;
        }
    }
    return matches;
}

std::map<int, ChangesMap> State::getByExtra(QVariantList extra)
{
    std::map<int, ChangesMap> matches;
    for(auto && change : changes) {

        for(auto && item : change.second) {
            for(auto && ext : extra) {
                if(item.extra.contains(ext)) {
                    matches[change.first] = item;
                    break;
                }
            }
        }
    }
    return matches;
}
