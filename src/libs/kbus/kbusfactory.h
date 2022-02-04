/*!
    \brief Factory for creating KBus implementations
    \authors oleksii, VladyslavN
    \version 0.1
    \date 26.04.2021
*/

#ifndef KBUSFACTORY_H
#define KBUSFACTORY_H

#include <functional>
#include "base/KBus.h"
#include "KBus_global.h"

#include <QSerialPort>
#include <vdebug.h>

class KBUS_EXPORT KBusFactory
{
private:
    KBusFactory() = default;

    struct CreateInfo {
        std::function<KBus*(QVariantMap params)> creator;
        std::function<QList<KBus::ConnectionData>()> getConnectionData;
    };

    QMap<QString, CreateInfo> m_creators;

    void installDefaultCreators();

    QMap<QString, QSerialPort::Parity> parityMap = {{"NoParity", QSerialPort::NoParity}, {"EvenParity", QSerialPort::EvenParity}, {"OddParity", QSerialPort::OddParity}};


public:

    static KBusFactory *instance();
    void instalBusCreator(QString type, std::function<KBus *(QVariantMap)> creator, std::function<QList<KBus::ConnectionData>()> getConnectionData);
    KBus* create(QString name, QVariantMap);
    template<class TClass> TClass* create(QString type, QVariantMap params) {return dynamic_cast<TClass*>(create(type, params));}

    QList<QString> getCreatorsNames();
    QList<KBus::ConnectionData> getBusCreateData(QString name);
};

#endif // KBUSFACTORY_H
