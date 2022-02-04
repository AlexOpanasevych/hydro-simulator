#ifndef STATECOMMUNICATOR_H
#define STATECOMMUNICATOR_H

#include <Protocol.h>
#include <QObject>
#include <QQueue>
#include <QVariant>
#include <kmacro.h>

class ClientService;
class MGKItem;

struct ResourceData {
    int stateId;
    QVariant value;
    QString station;
    QString action;
};

class StateCommunicator : public QObject
{
    Q_OBJECT
    K_QML_TYPE(StateCommunicator)
public:
    explicit StateCommunicator(ClientService *service, QObject *parent = nullptr);
    explicit StateCommunicator(QObject *parent = nullptr);

//    void sendValueToServer(QVariant value);

    void receiveFromServer(int stateId, QVariant value);

    void setCService(ClientService *value);
signals:
    void requestValueChanged(QVariant value, MGKItem*);

private:
    QList<ResourceData> m_mgkItems;
//    QQueue<ResourceData> sendQueue;

    ClientService * cService = nullptr;
};

#endif // STATECOMMUNICATOR_H
