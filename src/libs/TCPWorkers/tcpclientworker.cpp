#include "tcpclientworker.h"

#include <QDataStream>
#include <QDateTime>
#define MAX_CLIENTS_COUNT 6

TcpClientWorker::TcpClientWorker(QString address, int port, bool toConnect, QObject *parent) : QObject(parent)
{
    m_socket = new QTcpSocket;

    setAddress(address);
    setPort(port);

    QObject::connect(m_socket,SIGNAL(readyRead()),this,SLOT(readSocket()));
    QObject::connect(m_socket,SIGNAL(disconnected()),this,SLOT(discardSocket()));
    QObject::connect(m_socket, SIGNAL(connected()), this, SLOT(connect()));
    if(toConnect) connectToServer();
}

void TcpClientWorker::writeData(Frame & frame)
{
    sendData(frame.data);
}

TcpClientWorker::~TcpClientWorker()
{

    if(m_socket->isOpen())
        m_socket->close();
    m_socket->deleteLater();

}

void TcpClientWorker::readSocket()
{
    while(m_socket->bytesAvailable())
    {
//            QByteArray buffer;
        Frame frame;
        frame.type = SendType::P2P;
        frame.socketDescriptor = m_socket->socketDescriptor();

        int dataSize;
        m_socket->read((char*)&dataSize, sizeof(int));
        frame.data = m_socket->read(dataSize);

        while(frame.data.size() < dataSize) // only part of the message has been received
        {
            m_socket->waitForReadyRead(); // alternatively, store the buffer and wait for the next readyRead()
            frame.data.append(m_socket->read(dataSize - frame.data.size())); // append the remaining bytes of the message
        }

//            QString msg(buffer); // data in this case is JSON, so we can use a QString
//        qDebug() << frame.data.toHex('.');

        // Do something with the message
//        messageHandler(frame);
        if(m_receiver != nullptr) {
            m_receiver(frame);
            //emit newMessage(frame.data, socket());
        }
    }

}

void TcpClientWorker::discardSocket()
{
    qDebug() << "Disconnected!" << qobject_cast<QTcpSocket*>(sender())->socketDescriptor();
    m_socket->close();
//    m_socket->deleteLater();
//    m_socket = nullptr;

}

void TcpClientWorker::sendData(QByteArray &data)
{
//    int size = data.size();
//    data.push_front(QByteArray((char*)&size, sizeof (int)));

//    case SendType::Broadcast: {
//        for (QTcpSocket* socket : m_clientsList)
//        {
//            sendData(socket, data);
//        }
//        break;
//    }
    if(m_socket)
    {
        if(m_socket->isOpen())
        {
            const char * s = (const char*)data;
            m_socket->write(s, data.size());
        }
        else
            qDebug() << "QTCPServer: Socket doesn't seem to be opened";
    }
    else
        qDebug() << "Error: Not connected";
}

void TcpClientWorker::messageHandler(const QByteArray &block, QTcpSocket * socket)
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

void TcpClientWorker::stopServer()
{
    QTextStream os(m_socket);
    os.setAutoDetectUnicode(true);
    os << QDateTime::currentDateTime().toString() << "\n";

    m_socket->close();
}

void TcpClientWorker::connect()
{
    if(onConnectionPerformed != nullptr)
        onConnectionPerformed();
    else qDebug() << "on connection not set: fatal error";
}

QString TcpClientWorker::getAddress() const
{
    return m_address;
}

int TcpClientWorker::getPort() const
{
    return port;
}

void TcpClientWorker::setPort(int value)
{
    port = value;
}

QString TcpClientWorker::address() const
{
    return m_address;
}

QTcpSocket *TcpClientWorker::socket() const
{
    return m_socket;
}

void TcpClientWorker::setAddress(QString address)
{
    m_address = address;
}

bool TcpClientWorker::connectToServer()
{
    m_socket->connectToHost(QHostAddress(m_address), port);

    if(m_socket->waitForConnected(20)) {
        qDebug() << "Connected to Server";
        return true;
    }
    else{
        qDebug() << QString("The following error occurred: %1.").arg(m_socket->errorString());
        return false;
    }
}

void TcpClientWorker::setOnConnectionPerformed(const std::function<void ()> &value)
{
    onConnectionPerformed = value;
}

void TcpClientWorker::setReceiver(const std::function<void (const Frame &)> &receiver)
{
    m_receiver = receiver;
}

void TcpClientWorker::setSender(const std::function<void (const Frame &)> &sender)
{
    m_sender = sender;
}
