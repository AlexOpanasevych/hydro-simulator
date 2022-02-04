#include "streamconnectorclientside.h"

#include <QDataStream>
#include <QtConcurrent>

StreamConnectorClientSide* StreamConnectorClientSide::pinstance_{nullptr};
std::mutex StreamConnectorClientSide::mutex_;

StreamConnectorClientSide::StreamConnectorClientSide(QObject *parent) : QObject(parent), groupAddress("239.255.43.21")
{

}

StreamConnectorClientSide::~StreamConnectorClientSide()
{
    for(auto && key : streamSockets) {
        streamSockets.erase(key.first);
        key.first->close();
        delete key.first;
    }

    streamSockets.clear();
}


void StreamConnectorClientSide::bindToStream(const std::string &name, std::function<void (QVariantMap)> onReceived)
{
    qDebug() << "binding to stream" << name.c_str();

//    newSocket->bind(QHostAddress("192.168.81.112"), );
//    connect(newSocket, &QUdpSocket::readyRead, this, &StreamConnector::onReadyRead);
    if(handlersNames[name]) {
        handlersNames[name]->handlers.push_back(onReceived);
    }
    else {
        bindQueue.push({name.c_str(), onReceived});
        uniqueNames.push_back(name.c_str());
    }

}

void StreamConnectorClientSide::addStreamReceiver(int port, const std::string & name)
{
    qDebug() << "--- adding stream : receiver ---";
    QUdpSocket * newSocket = new QUdpSocket;
    StreamHandler * newHandler = new StreamHandler;

    newHandler->port = port;
    newHandler->socket = newSocket;
    newHandler->name = name;
//    newHandler->setMode(StreamHandler::Receiver);

    newSocket->bind(QHostAddress::AnyIPv4, port, QAbstractSocket::ShareAddress);
    newSocket->joinMulticastGroup(groupAddress);

    connect(newSocket, &QUdpSocket::readyRead, this, &StreamConnectorClientSide::onReadyRead);

    streamSockets[newSocket] = newHandler;
    handlersNames[name] = newHandler;

    for(size_t i = 0; i < bindQueue.size(); i++) {
        auto elem = bindQueue.front();
        if(elem.first.toStdString() == name) {
            // name matches with desired
            qDebug() << "adding handler";
            newHandler->handlers.push_back(elem.second);
        }
        else {
            bindQueue.push(elem);
        }
        bindQueue.pop();
    }
}

void StreamConnectorClientSide::onReadyRead()
{
//    qDebug() << "ready read";
    QUdpSocket * sender = qobject_cast<QUdpSocket*>(QObject::sender());

    auto result = streamSockets[sender];
    if(result) {
        while (sender->hasPendingDatagrams()) {
//            qDebug() << "reading data";
            QByteArray data;
            data.resize(sender->pendingDatagramSize());
            sender->readDatagram(data.data(), data.size());
            QDataStream s(data);
            QVariantMap values;
            s >> values;

            futures.resize(result->handlers.size());

            for(size_t i = 0; i < futures.size(); i++) {
                futures[i] = QtConcurrent::run([result, i, values] () {
                    result->handlers[i](values);
                });
            }

            for (auto && future : futures) {
                future.waitForFinished();
            }

        }
    }
}
