#ifndef TCPSERVERWORKER_H
#define TCPSERVERWORKER_H

#include "TCPWorkers_global.h"

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <Protocol.h>
//enum class SendType {P2P, Broadcast};
//struct TCPWORKERS_EXPORT Frame {
//    int socketDescriptor;
//    QByteArray data;
//    SendType type;
//};

class TCPWORKERS_EXPORT TcpServerWorker : public QObject
{
    Q_OBJECT
public:
    explicit TcpServerWorker(QString address, int port, QObject *parent = nullptr);

    //!
    //! \brief Get clients list
    //! \return list of available and opened sockets
    //!
    QList<QTcpSocket *> clientsList() const;

    ~TcpServerWorker();

    //!
    //! \brief Get all socket descriptors
    //! \return socket descriptors list
    //!
    QList<int> socketDescriptors() const;

    //!
    //! \brief Writes data to socket
    //!
    void writeData(Frame &);

    //!
    //! \brief Reads data from socket
    //! \param ok
    //! \return Frame object
    //!
    Frame readData(bool *ok);

    void setSender(const std::function<void (const Frame &)> &sender);

    void setReceiver(const std::function<void (const Frame &)> &receiver);

    //!
    //! \brief Gets socket by given \a index
    //! \param index
    //! \return QTcpSocket pointer
    //!
    QTcpSocket * socket(int index);

    //!
    //! \brief Gets client count
    //! \return client count
    //!
    int clientCount();

    //!
    //! \brief Gets socket by IP address
    //! \param addr
    //! \return QTcpSocket pointer to socket
    //!
    QTcpSocket * getSocketByAddr(const QString & addr);

    //!
    //! \brief Gets server address
    //! \return IP address string
    //!
    QString getAddress() const;

    //!
    //! \brief Set address
    //! \param address
    //!
    void setAddress(const QString &address);

    int getPort() const;
    void setPort(int value);

    void setMapping(const QString & id, QTcpSocket * socket);
    void setMapping(int socketDescriptor, const QString & id);

    void sendDataByID(const QString & id, QByteArray &data);

private slots:
    //!
    //! \brief newConnection
    //!
    void newConnection();

    //!
    //! \brief appendToClientsList
    //! \param socket
    //! \return
    //!
    bool appendToClientsList(QTcpSocket * socket);

    //!
    //! \brief readSocket
    //!
    void readSocket();

    //!
    //! \brief discardSocket
    //!
    void discardSocket();

    //!
    //! \brief sendData
    //! \param socket
    //! \param data
    //!
    void sendData(QTcpSocket* socket, const QByteArray & data);

    //!
    //! \brief sendData
    //! \param data
    //! \param receiverID
    //! \param type
    //!
    void sendData(QByteArray &data, int receiverID, SendType type);

    //!
    //! \brief messageHandler
    //! \param block
    //! \param socket
    //!
    void messageHandler(const QByteArray & block, QTcpSocket *socket);

    //!
    //! \brief stopServer
    //!
    void stopServer();


signals:

    //!
    //! \brief newMessage
    //!
    void newMessage(const QByteArray &, QTcpSocket *);

    //!
    //! \brief newConnection
    //! \param ip
    //! \param port
    //!
    void newConnection(QHostAddress ip, int port);

    //!
    //! \brief connectionRemoved
    //! \param socketDescriptor
    //!
    void connectionRemoved(int socketDescriptor);
private:

    QTcpServer * m_server = nullptr;

    QList<QTcpSocket*> m_clientsList;

    QList<int> m_socketDescriptors;
    QMap<QString, QTcpSocket*> mappingToSocket;
    QMap<QTcpSocket*, QString> mappingToID;

    std::function<void(const Frame&)> m_sender;
    std::function<void(const Frame&)> m_receiver;

    QString m_address = "127.0.0.1";
    int port;

};

#endif // TCPSERVERWORKER_H
