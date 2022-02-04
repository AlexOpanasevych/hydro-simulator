#ifndef STREAMCONNECTORV2_H
#define STREAMCONNECTORV2_H

#include <QFuture>
#include <QObject>
#include <QQueue>
#include <QUdpSocket>
#include <mutex>
#include <TCPWorkers_global.h>

//define USE_AS_RECEIVER to use StreamConnector as Udp client

typedef std::function<void (const QVariant &)> token;
typedef std::shared_ptr<token> listener;

//!
//! \brief The UdpSenderWorker class
//!
class TCPWORKERS_EXPORT UdpSenderWorker : public QObject {
    Q_OBJECT
    QUdpSocket * socket;
    QHostAddress address;
    int port;
    Q_PROPERTY(QString streamName READ streamName WRITE setStreamName NOTIFY streamNameChanged)
private:

    QString m_streamName;

public:
    UdpSenderWorker(const QString & name, const QHostAddress & address, int port);
    ~UdpSenderWorker();

    QString streamName() const;

public slots:
//    void process(); 	/*  создает и запускает построитель отчетов */
//    void stop();    	/*  останавливает построитель отчетов */
    void init();
    void writeData(const QVariant & data);

    void setStreamName(QString streamName);
    void stop();
signals:
    void streamNameChanged(QString streamName);
//    void readyRead();
    void disconnected();
    void connected();
    void error();
};




//!
//! \brief The UdpReceiverWorker class
//!
class TCPWORKERS_EXPORT UdpReceiverWorker : public QObject {
    Q_OBJECT
    QUdpSocket * socket;
    QHostAddress address;
    int port;
    QVector<std::weak_ptr<token>> handlers;
    QVector<QFuture<void>> futures;
    Q_PROPERTY(QString streamName READ streamName WRITE setStreamName NOTIFY streamNameChanged)
private:

    QString m_streamName;

public:
    UdpReceiverWorker(const QString & name, const QHostAddress & address, int port);
    ~UdpReceiverWorker();

    void addHandler(std::weak_ptr<token> handler);

    void removeHandler(std::shared_ptr<token> handler);

    QString streamName() const;

    QUdpSocket *getSocket() const;

    QHostAddress getAddress() const;

    int getPort() const;

private slots:
    void readData();
public slots:
    void init();

    void setStreamName(QString streamName);
    void stop();

    int handlersCount() {return handlers.size();}
signals:
    void streamNameChanged(QString streamName);
    void readyRead();
    void disconnected();
    void connected();
    void error();
};








//!
//! \brief The StreamConnectorV2Sender class
//!
class TCPWORKERS_EXPORT StreamConnectorV2Sender : public QObject
{
    Q_OBJECT
    static StreamConnectorV2Sender * pinstance_;
    static std::mutex mutex_;
    explicit StreamConnectorV2Sender(QObject *parent = nullptr);
public:

    static StreamConnectorV2Sender * instance() {
        std::lock_guard<std::mutex> lock(mutex_);
            if (pinstance_ == nullptr)
            {
                pinstance_ = new StreamConnectorV2Sender;
            }
            return pinstance_;
    }
    ~StreamConnectorV2Sender();
    void addStreamSender(const QString & name);
signals:
    void stopAll();

private:
    const int startPort = 44450;
    int currentPort = startPort;

    QVector<int> availablePorts;
    QStringList m_uniqueBindNames;
    QQueue<QPair<QUdpSocket*, UdpSenderWorker*>> requestQueue;
    QVector<QFuture<void>> futures;

    QMap<QString, UdpSenderWorker*> streamSockets;


    QHostAddress groupAddress;
public slots:
    void sendData(const QVariant &data, const QString &name);
};







//!
//! \brief The StreamConnectorV2Receiver class
//!
class TCPWORKERS_EXPORT StreamConnectorV2Receiver : public QObject
{
    Q_OBJECT
    static StreamConnectorV2Receiver * pinstance_;
    static std::mutex mutex_;
    explicit StreamConnectorV2Receiver(QObject *parent = nullptr);
public:

    static StreamConnectorV2Receiver * instance() {
        std::lock_guard<std::mutex> lock(mutex_);
            if (pinstance_ == nullptr)
            {
                pinstance_ = new StreamConnectorV2Receiver;
            }
            return pinstance_;
    }
    ~StreamConnectorV2Receiver();
public:
    std::shared_ptr<token> bindToStream(const QString & name, token handler);
    QStringList uniqueBindNames();
    QByteArray writeRequestAdd(QString name);
private:
    void addStreamReceiver();
    QQueue<QPair<QString, std::weak_ptr<token>>> bindQueue;
signals:
    void stopAll();

private:
    const int startPort = 44450;
    int currentPort = startPort;

//    QVector<int> availablePorts;
    QStringList m_uniqueBindNames;
    QQueue<QPair<QUdpSocket*, UdpSenderWorker*>> requestQueue;
    QVector<QFuture<void>> futures;

    QMap<QString, UdpReceiverWorker*> streamSockets;

    QHostAddress groupAddress;
public slots:
};

#endif // STREAMCONNECTORV2_H
