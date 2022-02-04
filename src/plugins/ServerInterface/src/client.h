#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <kmacro.h>
#include "../serverinterface_global.h"
class Mode;

class SERVERINTERFACE_LIBRARY_EXPORT Client : public QObject
{
    Q_OBJECT
    K_QML_TYPE(Client)
    K_AUTO_PROPERTY(Mode*, mode, mode, setMode, modeChanged, nullptr);
    K_AUTO_PROPERTY(int, absoluteId, absoluteId, setAbsoluteId, absoluteIdChanged, 0)
//    K_AUTO_PROPERTY(int, relativeId, relativeId, setRelativeId, relativeIdChanged, 0)
    K_AUTO_PROPERTY(int, socketDescriptor, socketDescriptor, setSocketDescriptor, socketDescriptorChanged, 0)
//    K_AUTO_PROPERTY(int, type, type, setType, typeChanged, 0)
    K_AUTO_PROPERTY(QString, uid, uid, setUid, uidChanged, 0)
    K_AUTO_PROPERTY(QString, addr, addr, setAddr, addrChanged, "")
    K_AUTO_PROPERTY(int, port, port, setPort, portChanged, 0)
    K_AUTO_PROPERTY(bool, alive, isAlive, setAlive, aliveChanged, false)
    K_AUTO_PROPERTY(bool, connected, connected, setConnected, connectedChanged, false)

//    QString toString();
public:
    explicit Client(QObject *parent = nullptr);
    ~Client();
signals:
private:
};

#endif // CLIENT_H
