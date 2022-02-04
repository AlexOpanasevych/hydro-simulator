#ifndef TCPWORKERS_H
#define TCPWORKERS_H

#include "Protocol.h"
#include "TCPWorkers_global.h"

#include <QHostAddress>
#include <QObject>
#include <QTcpSocket>

class TCPWORKERS_EXPORT TcpClientWorker : public QObject
{
    Q_OBJECT
public:

    explicit TcpClientWorker(QString address, int port, bool toConnect = true, QObject *parent = nullptr);
    ~TcpClientWorker();

    //!
    //! \brief Writes data with given \a frame
    //!
    void writeData(Frame & frame);

    //!
    //! \brief Reads data from socket
    //! \param ok
    //! \return Frame object which contains data, socket descriptor, and type of connection
    //!
    Frame readData(bool *ok);

    //!
    //! \brief Sets callback on send routine
    //! \param sender
    //!
    void setSender(const std::function<void (const Frame &)> &sender);

    //!
    //! \brief Sets callback on receive routine
    //! \param receiver
    //!
    void setReceiver(const std::function<void (const Frame &)> &receiver);

    //!
    //! \brief Sets callback on ConnectionPerformed routine
    //! \param value
    //!
    void setOnConnectionPerformed(const std::function<void ()> &value);

    //!
    //! \brief socket
    //! \return socket binded with \a this client
    //!
    QTcpSocket *socket() const;

    //!
    //! \brief Sets new IP address
    //! \return
    //!
    void setAddress(QString address);

    //!
    //! \brief Connects to server with \a m_adress
    //! \return \a true if succeed, \a false if not
    //!
    Q_INVOKABLE bool connectToServer();

    QString address() const;

    int getPort() const;
    void setPort(int value);

    QString getAddress() const;

private slots:

    //!
    //! \brief Reads all available data from socket
    //!
    void readSocket();

    //!
    //! \brief Discards socket when connection is lost
    //!
    void discardSocket();

    //!
    //! \brief Sends \a data to server
    //! \param data
    //!
    void sendData(QByteArray &data);

    //!
    //! \brief Handles new \a block of data with given \a socket
    //! \param block
    //! \param socket
    //!
    void messageHandler(const QByteArray & block, QTcpSocket *socket);


    void stopServer();

    //!
    //! \brief Connect routine
    //!
    void connect();

private:

    QTcpSocket * m_socket = nullptr;

    QString m_address = "127.0.0.1";
    int port = 33333;

    //QList<QTcpSocket*> m_clientsList;

    //QList<int> m_socketDescriptors;

//    KCircleBuffer<Frame> sendBuffer;
//    KCircleBuffer<Frame> receiveBuffer;

    std::function<void()> onConnectionPerformed;

    std::function<void(const Frame&)> m_sender;
    std::function<void(const Frame&)> m_receiver;

};

#endif // TCPWORKERS_H
