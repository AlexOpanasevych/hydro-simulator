#ifndef STREAMCONNECTORCLIENTSIDE_H
#define STREAMCONNECTORCLIENTSIDE_H

#include <QObject>
#include <QUdpSocket>
#include <mutex>
#include "TCPWorkers_global.h"
#include <queue>
#include <QFuture>

struct TCPWORKERS_EXPORT StreamHandler {
    QUdpSocket * socket;
    std::string name;
    int port;
    std::vector<std::function<void(QVariantMap)>> handlers;
};

class TCPWORKERS_EXPORT StreamConnectorClientSide : public QObject
{
    Q_OBJECT
    static StreamConnectorClientSide * pinstance_;
    static std::mutex mutex_;
    explicit StreamConnectorClientSide(QObject *parent = nullptr);
public:
    static StreamConnectorClientSide * instance() {
        std::lock_guard<std::mutex> lock(mutex_);
            if (pinstance_ == nullptr)
            {
                pinstance_ = new StreamConnectorClientSide;
            }
            return pinstance_;
    }
    ~StreamConnectorClientSide();

    QVector<QString> uniqueBindNames() {return uniqueNames;}

    std::map<QUdpSocket*, StreamHandler*> streamSockets;
    std::map<std::string, StreamHandler*> handlersNames;
    std::queue<QPair<QString, std::function<void(QVariantMap)>>> bindQueue;
    QVector <QString> uniqueNames;

    QHostAddress groupAddress;
    std::vector<QFuture<void>> futures;

signals:
public slots:
    void bindToStream(const std::string & name, std::function<void(QVariantMap)> onReceived);
    void addStreamReceiver(int port, const std::string &name); // for sending
    void onReadyRead();
};




#endif // STREAMCONNECTORCLIENTSIDE_H
