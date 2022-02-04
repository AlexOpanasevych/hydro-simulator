#include "streamconnectorv2.h"

#include <QDataStream>
#include <QThread>
#include <QtConcurrent>
#include <Protocol.h>

StreamConnectorV2Sender::StreamConnectorV2Sender(QObject *parent) : QObject(parent)
{

}

StreamConnectorV2Sender::~StreamConnectorV2Sender()
{

}

void StreamConnectorV2Sender::addStreamSender(const QString &name)
{

    if(streamSockets.find(name) != streamSockets.end()) {
        int port;
        if(!availablePorts.empty())
            port = availablePorts.takeLast();
        else
            port = currentPort++;

        UdpSenderWorker* worker = new UdpSenderWorker(name, groupAddress, port);
        QThread* thread = new QThread;
        worker->moveToThread(thread);

        connect(thread, &QThread::started, worker, &UdpSenderWorker::init);
//         … и при запуске потока будет вызван метод process(), который создаст построитель отчетов, который будет работать в новом потоке

//        connect(worker, SIGNAL(finished()), thread, SLOT(quit()));
////         … и при завершении работы построителя отчетов, обертка построителя передаст потоку сигнал finished() , вызвав срабатывание слота quit()

            connect(this, &StreamConnectorV2Sender::stopAll, worker, &UdpSenderWorker::stop);
//         … и Session может отправить сигнал о срочном завершении работы обертке построителя, а она уже остановит построитель и направит сигнал finished() потоку

//            connect(worker, SIGNAL(finished()), worker, SLOT(deleteLater()));
////         … и обертка пометит себя для удаления при окончании построения отчета

            connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
// … и поток пометит себя для удаления, по окончании построения отчета. Удаление будет произведено только после полной остановки потока.

            thread->start();
// Запускаем поток, он запускает RBWorker::process(), который создает ReportBuilder и запускает  построение отчета */

    }
}

void StreamConnectorV2Sender::sendData(const QVariant &data, const QString &name)
{
    auto worker = streamSockets.find(name);
    if(worker != streamSockets.end()) {
        QByteArray dataArr;
        QDataStream s(&dataArr, QIODevice::WriteOnly);
        s << data;
        (*worker)->writeData(dataArr);
    }
}

StreamConnectorV2Receiver::~StreamConnectorV2Receiver()
{

}

std::shared_ptr<token> StreamConnectorV2Receiver::bindToStream(const QString &name, token handler)
{
    qDebug() << "{INFO} binding to stream" << name;

    auto ptr = std::make_shared<token>(std::move(handler));

    if(streamSockets[name]) {
        qDebug() << streamSockets[name]->handlersCount();
        streamSockets[name]->addHandler(ptr);
    }
    else {
        bindQueue.enqueue({name, ptr});
        m_uniqueBindNames.push_back(name);
    }
    return ptr;
}

QStringList StreamConnectorV2Receiver::uniqueBindNames()
{
    return m_uniqueBindNames;
}

void UdpSenderWorker::init()
{
    QUdpSocket * newSocket = new QUdpSocket;

    connect(newSocket, &QUdpSocket::connected, this, &UdpSenderWorker::connected);
    connect(newSocket, &QUdpSocket::disconnected, this, &UdpSenderWorker::disconnected);
    connect(newSocket, &QUdpSocket::errorOccurred, this, &UdpSenderWorker::error);

}

void UdpReceiverWorker::readData()
{
    while (socket->hasPendingDatagrams()) {
        QByteArray data;
        data.resize(socket->pendingDatagramSize());
        socket->readDatagram(data.data(), data.size());
        QDataStream s(data);
        QVariant values;
        s >> values;

        futures.resize(handlers.size());

        for(int i = 0; i < futures.size(); i++) {
            futures[i] = QtConcurrent::run([this, i, values] () {
                auto ptr = handlers[i].lock();
                if(ptr)
                    (*ptr)(values);
            });
        }

        for (auto && future : futures) {
            future.waitForFinished();
        }
    }
}

void UdpSenderWorker::writeData(const QVariant &data)
{
    QByteArray dataArr;
    QDataStream s(&dataArr, QIODevice::WriteOnly);
    s << data;
    socket->writeDatagram( dataArr, address, port );
}

void UdpSenderWorker::stop()
{
    socket->close();
    socket->deleteLater();
}

UdpSenderWorker::UdpSenderWorker(const QString &name, const QHostAddress &address, int port) : address(address), port(port)
{
    setStreamName(name);
}

UdpSenderWorker::~UdpSenderWorker()
{
    stop();
}

UdpReceiverWorker::~UdpReceiverWorker()
{
    stop();
}

void UdpReceiverWorker::addHandler(std::weak_ptr<token> handler)
{
    auto result = std::find_if(handlers.begin(), handlers.end(), [&](const auto & h){
        return h.lock() == handler.lock();
    });
    if(result == handlers.end()) {
        handlers.push_back(handler);
    }
}

QByteArray StreamConnectorV2Receiver::writeRequestAdd(QString name)
{
    QByteArray requestData;
    QDataStream stream(&requestData, QIODevice::WriteOnly);
    auto socketWorker = streamSockets.find(name);
    if(socketWorker != streamSockets.end()) {
        stream << ADD_STREAM << (*socketWorker)->getPort() << name;
        return requestData;
    }
    return {};
}


QString UdpReceiverWorker::streamName() const
{
    return m_streamName;
}

QUdpSocket *UdpReceiverWorker::getSocket() const
{
    return socket;
}

QHostAddress UdpReceiverWorker::getAddress() const
{
    return address;
}

int UdpReceiverWorker::getPort() const
{
    return port;
}

void UdpReceiverWorker::init()
{
    QUdpSocket * newSocket = new QUdpSocket;

    connect(newSocket, &QUdpSocket::connected, this, &UdpReceiverWorker::connected);
    connect(newSocket, &QUdpSocket::disconnected, this, &UdpReceiverWorker::disconnected);
    connect(newSocket, &QUdpSocket::errorOccurred, this, &UdpReceiverWorker::error);
    connect(newSocket, &QUdpSocket::readyRead, this, &UdpReceiverWorker::readData);
}

void UdpReceiverWorker::stop()
{
    socket->close();
    socket->deleteLater();
}


void UdpSenderWorker::setStreamName(QString streamName)
{
    if (m_streamName == streamName)
        return;

    m_streamName = streamName;
    emit streamNameChanged(m_streamName);
}

void UdpReceiverWorker::setStreamName(QString streamName)
{
    if (m_streamName == streamName)
        return;

    m_streamName = streamName;
    emit streamNameChanged(m_streamName);
}


QString UdpSenderWorker::streamName() const
{
    return m_streamName;
}
