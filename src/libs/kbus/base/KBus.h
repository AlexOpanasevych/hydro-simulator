/*!
    \brief Base class for all KBus implementations
    \authors oleksii
    \version 0.1
    \date 26.04.2021
*/

#ifndef KBUS_H
#define KBUS_H

#include <QList>
#include <QObject>
#include <QString>
#include <functional>
#include <QByteArray>
#include <QVariantMap>

#include "kbusworker.h"
#include "KBus_global.h"

class KBUS_EXPORT KBus : public QObject
{
    Q_OBJECT

protected:
    QList<KBusWorker*> m_workers;
    virtual void startOverride() = 0;

public:
    struct ConnectionData {
        QString name;
        QString type;
        QVariant data;
    };

    KBus() {}
    virtual ~KBus() {}

    void start() { startOverride(); }

    virtual bool isConnected() = 0;

    virtual bool isEmptyTX() = 0;
    virtual bool isEmptyRX() = 0;

    virtual void push(const QByteArray &) = 0;
    virtual QByteArray pop() = 0;

    void addWorker(KBusWorker* w){ m_workers.push_back(w); }
    void removeAllWorker(KBusWorker* w){ m_workers.removeAll(w); } // Why function name is removeWorker but inside action remove all ?
};

#endif // KBUS_H
