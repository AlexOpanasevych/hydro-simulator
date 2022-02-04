#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QByteArray>
#include <mutex>
#include "TCPWorkers_global.h"

enum class SendType {P2P, Broadcast};
struct Frame {
    int socketDescriptor;
    QByteArray data;
    SendType type;
};

constexpr auto ADD_STREAM = uint8_t(30);

#endif // PROTOCOL_H
