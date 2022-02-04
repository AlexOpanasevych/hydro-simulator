#ifndef MGKFSM_H
#define MGKFSM_H

#include "src/abstractsubsystem.h"

#include <QJsonObject>
#include "../../serverinterface_global.h"
#include <utility/kobservablelist.h>

struct SERVERINTERFACE_LIBRARY_EXPORT ChangesMap {
    int stateId;
    QVariant value;
    QVariantList extra;
    QVariantMap valueChanges;
    QVariantMap execValues;
    bool hasExtra() {return !extra.isEmpty();}
};

struct SERVERINTERFACE_LIBRARY_EXPORT State {
    int stateId;
    QString name;
    std::map<int, std::vector<ChangesMap>> changes;
    void loadState(const QJsonValue & entityData);
    std::map<int, ChangesMap> getByValue(QVariant value);
    std::map<int, ChangesMap> getByExtra(QVariant extra);
    std::map<int, ChangesMap> getByExtra(QVariantList extra);

};

class MgkFSM;

struct SERVERINTERFACE_LIBRARY_EXPORT TransitionMap {
    std::map<int, State> states;
    void init(const QJsonArray & initData);
    State getByName(const QString &stateName, bool *ok);
    MgkFSM * m_stateMachine = nullptr;
    TransitionMap();
public:
    void setStateMachine(MgkFSM *stateMachine);
    bool checkForConditions(const QVariantMap &conditions);
};


class SERVERINTERFACE_LIBRARY_EXPORT MgkFSM : public AbstractSubsystem
{
    TransitionMap map;
public:
    MgkFSM(QObject * parent = nullptr);

    int order() override;

    // AbstractSubsystem interface
public slots:
    void proceed(KObservableList<Resource> &stateList, const StateData &data) override;
    void init() override;
};

#endif // MGKFSM_H


