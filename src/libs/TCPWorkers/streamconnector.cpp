#include "streamconnector.h"

#include <QDataStream>
#include <Protocol.h>
#include <QtConcurrent>
#include <QDebug>


StreamConnector* StreamConnector::pinstance_{nullptr};
std::mutex StreamConnector::mutex_;
std::mutex StreamConnector::handlerMutex;


void StreamConnector::setServer(bool value)
{
    server = value;
    emit streamModeChanged(value);
}

StreamConnector::StreamConnector(QObject *parent) : QObject(parent), groupAddress("239.255.43.21")
{
}

void StreamConnector::addStreamSender(const std::string & name, std::function<void (const QVariant &)> sender)
{
    Q_UNUSED(sender)
    if(handlersNames.find(name) == handlersNames.end()) {
        QUdpSocket * newSocket = new QUdpSocket(this);
        int port = 0;
        if(!availablePorts.empty()) {
            port = availablePorts.back();
            availablePorts.pop_back();
        }
        else {
            port = currentPort++;
        }
        newSocket->bind(QHostAddress(QHostAddress::AnyIPv4));
        qDebug() << "--- adding stream : sender" << name.c_str() << "---" << "port" << port;
        StreamHandler * newHandler = new StreamHandler;
        newHandler->port = port;

        //newHandler->handler.push_back(sender);
        newHandler->socket = newSocket;
//        newHandler->setMode(StreamHandler::Sender);

        handlersNames[name] = newHandler;
        streamSockets[newSocket] = newHandler;

//        emit requestAddStream(port, StreamHandler::Sender, name.c_str());

    }

//    requestQueue.enqueue({newSocket, newHandler});



//    streamSockets[newSocket] = newHandler; // removed because we send request to add stream
//    emit requestAddStream(port, StreamHandler::Receiver);
}

void StreamConnector::addStreamReceiver(int port, const std::string & name)
{
//    if(handlersNames.find(name) == handlersNames.end()) {
        qDebug() << "{INFO} adding stream : receiver for stream" << name.c_str() << port;
        QUdpSocket * newSocket = new QUdpSocket(this);
        StreamHandler * newHandler = new StreamHandler;

        newHandler->port = port;
        newHandler->socket = newSocket;
        newHandler->name = name;
    //    newHandler->setMode(StreamHandler::Receiver);

        connect(newSocket, &QUdpSocket::errorOccurred, this, [newSocket](){
            qDebug() << newSocket->errorString();
        });
    //    if(!newSocket->isOpen()) {
            newSocket->bind(QHostAddress::AnyIPv4, port, QAbstractSocket::ShareAddress);
            newSocket->joinMulticastGroup(groupAddress);
            connect(newSocket, &QUdpSocket::readyRead, this, &StreamConnector::onReadyRead);
    //    }


        streamSockets[newSocket] = newHandler;
        handlersNames[name] = newHandler;

        for(size_t i = 0; i < bindQueue.size(); i++) {
            auto elem = bindQueue.front();
            if(elem.first.toStdString() == name) {
                // name matches with desired
                qDebug() << "{INFO} adding handler to stream" << name.c_str();
                qDebug() << newHandler->handlers.size();
                newHandler->handlers.push_back(elem.second);
            }
            else {
                bindQueue.push(elem);
            }
            bindQueue.pop();
        }
//    }
}

std::shared_ptr<token> StreamConnector::bindToStream(const std::string &name, token onReceived)
{

    qDebug() << "{INFO} binding to stream" << name.c_str();

    auto ptr = std::make_shared<token>(std::move(onReceived));

    if(handlersNames[name]) {
        qDebug() << handlersNames[name]->handlers.size();
        handlersNames[name]->handlers.push_back(ptr);
    }
    else {
        bindQueue.push({name.c_str(), ptr});
        uniqueNames.push_back(name.c_str());
    }
    return ptr;
}

void StreamConnector::removeStream(QUdpSocket * sock)
{
    auto handler = streamSockets.take(sock);
    availablePorts.push_back(handler->port);
    sock->close();
    sock->deleteLater();
    delete handler;
}

void StreamConnector::onStreamAnotherSideAdded(QDataStream *stream)
{
    int port;
    int mode;
    *stream >> port >> mode;
    if(requestQueue.front().second->port == port) {
        auto dataStream = requestQueue.front();
        requestQueue.pop();
        streamSockets[dataStream.first] = dataStream.second;
        qDebug() << "{INFO} stream added on other side";
    }
    else qDebug() << "{ERROR} not same port: delivery isn't correct or server is broken or idk but many things may happen";
}

void StreamConnector::clearHandlers() {
    for(auto && key = streamSockets.begin(); key != streamSockets.end();) {
        auto handler = key.value();
        auto keyP = key.key();
        streamSockets.erase(key++);
        handlersNames.remove(handler->name);
        keyP->close();
        keyP->deleteLater();
        delete handler;
    }
}

//void StreamConnector::pushToStreamServer(std::string where, const QVariant &value)
//{
//    sendData(data)
//}

QVector<QString> StreamConnector::uniqueBindNames()
{
    return uniqueNames;
}

//void StreamConnector::setMode(StreamHandler::Mode mode)
//{
//    if (m_mode == mode)
//        return;

//    m_mode = mode;
//    emit modeChanged(m_mode);
//}

void StreamConnector::onReadyRead()
{
    QUdpSocket * sender = qobject_cast<QUdpSocket*>(QObject::sender());

    auto result = streamSockets[sender];
    if(result) {
        while (sender->hasPendingDatagrams()) {
            QByteArray data;
            data.resize(sender->pendingDatagramSize());
            sender->readDatagram(data.data(), data.size());
            QDataStream s(data);
            QVariant values;
            s >> values;

            futures.resize(result->handlers.size());

            for(int i = 0; i < futures.size(); i++) {
                futures[i] = QtConcurrent::run([result, i, values] () {
                    std::lock_guard lock(handlerMutex);
                    auto ptr = result->handlers[i].lock();
                    if(ptr)
                        (*ptr)(values);
                });
            }

            for (auto && future : futures) {
                future.waitForFinished();
            }

        }
    }
}


StreamConnector::~StreamConnector()
{
}

QByteArray StreamConnector::writeRequestAdd(std::string name)
{
    QByteArray requestData;
    QDataStream stream(&requestData, QIODevice::WriteOnly);
    if(handlersNames.find(name) != handlersNames.end()) {
        stream << ADD_STREAM << handlersNames[name]->port << QString(name.c_str());
        return requestData;
    }
    return {};
}

void StreamConnector::sendData(const QVariant &data, const std::string & name)
{
    if(handlersNames.find(name) != handlersNames.end()) {
        //qDebug() << "writing data, port:" << handlersNames[name]->port << data;

        auto socket = handlersNames[name]->socket;
        QByteArray dataArr;
        QDataStream s(&dataArr, QIODevice::WriteOnly);
        s << data;
        socket->writeDatagram(dataArr, groupAddress, handlersNames[name]->port);
    }
}
