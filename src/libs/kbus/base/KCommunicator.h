/*!
    \brief Base class for all KBus Communicators
    \authors oleksii
    \version 0.1
    \date 26.04.2021
*/

#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QByteArray>
#include <functional>

#include "KBus_global.h"
#include "vdebug.h"

class KBUS_EXPORT KCommunicator
{
protected:
    std::function<void(const QByteArray&)> receiveCallback  = nullptr;
    std::function<void(const QByteArray&)> transmitCallback = nullptr;

public:
    KCommunicator() {};
    virtual ~KCommunicator() {};

    virtual void send(QByteArray &) = 0;
    virtual void proceedRawData(const QByteArray &) = 0;

    void setReceivePackCallback (std::function<void(const QByteArray&)> receiveCallback) {
        if (receiveCallback != nullptr) {
            this->receiveCallback = receiveCallback;
        } else {
            VDebug::Error("Communicator can`t set null pointer callback for receive data");
        }
    };

    void setTransmitPackCallback(std::function<void(const QByteArray&)> transmitCallback) {
        if (transmitCallback != nullptr) {
            this->transmitCallback = transmitCallback;
        } else {
            VDebug::Error("Communicator can`t set null pointer callback for transmit data");
        }
    };
};

#endif // COMMUNICATOR_H
