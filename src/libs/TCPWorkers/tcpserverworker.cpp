#include "tcpserverworker.h"

#include <QDataStream>
#include <QDateTime>
#include <QProcess>
#include <cdebug.h>
#define MAX_CLIENTS_COUNT 6

TcpServerWorker::TcpServerWorker(QString address, int port, QObject *parent) : QObject(parent)
{
    m_server = new QTcpServer;
    setAddress(address);
    setPort(port);
    if(m_server->listen(QHostAddress(m_address), port))
    {
        connect(this,SIGNAL(newMessage( QByteArray, QTcpSocket*)),this,SLOT(messageHandler(const QByteArray &, QTcpSocket *)));
        connect(m_server, SIGNAL(newConnection()), this, SLOT(newConnection()));

        qDebug() << "Server is listening on ip" << m_server->serverAddress().toString();
    }
    else
    {
//        QString nParameter = "-n";
//        QString pingCount = "1"; //(int)
//        QString wParameter = "-w";
//        QString pingWaitTime = "10"; //(ms)
        //QProcess * pingProcess = new QProcess;
        //int exitCode = pingProcess->execute("ping", QStringList("192.168.81.112") << nParameter << pingCount << wParameter << pingWaitTime);
        //if (exitCode==1){
        //    qDebug() << "zanyato";
        //}
        //else {
        //
        //    qDebug() << "ne zanyato";
        //}
        //qDebug() << "CRITICAL ERROR: CANNOT USE PORT, EXITING";
        //exit(EXIT_FAILURE);
    }
}


QTcpSocket *TcpServerWorker::getSocketByAddr(const QString &addr)
{
    for(auto && socket : m_clientsList) {
        if(socket->peerAddress().toString() == addr) {
            return socket;
        }
    }
    return nullptr;
}

QList<int> TcpServerWorker::socketDescriptors() const
{
    return m_socketDescriptors;
}

void TcpServerWorker::writeData(Frame & frame)
{
    sendData(frame.data, frame.socketDescriptor, frame.type);
}

QList<QTcpSocket *> TcpServerWorker::clientsList() const
{
    return m_clientsList;
}

TcpServerWorker::~TcpServerWorker()
{
    for (auto & client: m_clientsList) {
        client->close();
        client->deleteLater();
    }

    m_server->close();
    m_server->deleteLater();

}

void TcpServerWorker::newConnection()
{
    while (m_server->hasPendingConnections()) {
        appendToClientsList(m_server->nextPendingConnection());
    }
}

bool TcpServerWorker::appendToClientsList(QTcpSocket *socket)
{
    m_clientsList.append(socket);
    m_socketDescriptors.push_back(socket->socketDescriptor());
    connect(socket, SIGNAL(readyRead()), this , SLOT(readSocket()));
    connect(socket, SIGNAL(disconnected()), this , SLOT(discardSocket()));

    emit newConnection(socket->peerAddress(), socket->peerPort());

    qDebug() << QString("INFO: Client with sockd: %1 has just entered the room").arg(socket->socketDescriptor());
    return true;
}

void TcpServerWorker::readSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());

    Frame frame;
    frame.socketDescriptor = socket->socketDescriptor();
    frame.data = socket->readAll();
    qDebug() << frame.data.toHex('.');

    emit newMessage(frame.data, socket);
}

void TcpServerWorker::discardSocket()
{
    QTcpSocket* socket = reinterpret_cast<QTcpSocket*>(sender());
    int i = m_clientsList.indexOf(socket);
    emit connectionRemoved( m_socketDescriptors[i] );
    m_clientsList.removeAt(i);
    m_socketDescriptors.removeAt(i);

    QString id = mappingToID[socket];
    mappingToID.remove(socket);
    mappingToSocket.remove(id);

    socket->deleteLater();
}

void TcpServerWorker::sendData(QTcpSocket *socket, const QByteArray & data)
{
    if(socket)
    {
        if(socket->isOpen())
        {
            qDebug() << data;
            socket->write(data);
        }
        else
            qDebug() << "QTCPServer: Socket doesn't seem to be opened";
    }
    else
        qDebug() << "Error: Not connected";
}

void TcpServerWorker::sendData(QByteArray &data, int receiverID, SendType type)
{
    int size = data.size();
    data.push_front(QByteArray((char*)&size, sizeof (int)));

    switch(type) {
    case SendType::Broadcast: {
        for (auto & socket : m_clientsList)
        {
            sendData(socket, data);
        }
        break;
    }
    case SendType::P2P: {

        sendData(m_clientsList[m_socketDescriptors.indexOf(receiverID)], data);

        break;
    }
    }
}

void TcpServerWorker::messageHandler(const QByteArray &block, QTcpSocket * socket)
{
    QDataStream in(block);
//    in.setVersion(QDataStream::Qt_5_15);

    //    ServerSideParser parser;
    //    parser.parse(&in);
    //    QVariantList result = parser.captured();
    Frame frame;
    frame.data = block;
    frame.type = SendType::P2P;
    frame.socketDescriptor = socket->socketDescriptor();
    if(m_receiver != nullptr)
        m_receiver(frame);
    else qDebug() << "Critical Error: set receiver callback";
}

void TcpServerWorker::stopServer()
{
    for(auto & client : m_clientsList){
        qDebug() << QDateTime::currentDateTime().toString() << "\n";
        client->close();
        m_clientsList.removeOne(client);
        client->deleteLater();
    }
    m_server->close();
}

void TcpServerWorker::setMapping(const QString & id, QTcpSocket *socket)
{
    mappingToSocket[id] = socket;
    mappingToID[socket] = id;
}

void TcpServerWorker::setMapping(int socketDescriptor, const QString & id)
{
    auto sock = std::find_if(m_clientsList.begin(), m_clientsList.end(), [&](QTcpSocket * socket){
        return socket->socketDescriptor() == socketDescriptor;
    });
    mappingToID[*sock] = id;
    mappingToSocket[id] = *sock;

}

void TcpServerWorker::sendDataByID(const QString &id, QByteArray &data)
{
    qDebug(s) << mappingToSocket;
    auto result = mappingToSocket.find(id);
    if(result != mappingToSocket.end()) {
        qDebug(s) << "found socket" << (*result)->localAddress()<< (*result)->localPort();
        int size = data.size();
        data.push_front(QByteArray((char*)&size, sizeof (int)));
        sendData(result.value(), data);
    }

}

int TcpServerWorker::getPort() const
{
    return port;
}

void TcpServerWorker::setPort(int value)
{
    port = value;
}

QString TcpServerWorker::getAddress() const
{
    return m_address;
}

void TcpServerWorker::setAddress(const QString &address)
{
    m_address = address;
}

void TcpServerWorker::setReceiver(const std::function<void (const Frame &)> &receiver)
{
    m_receiver = receiver;
}

QTcpSocket *TcpServerWorker::socket(int index)
{
    return m_clientsList[index];
}

int TcpServerWorker::clientCount()
{
    return m_clientsList.size();
}

void TcpServerWorker::setSender(const std::function<void (const Frame &)> &sender)
{
    m_sender = sender;
}
