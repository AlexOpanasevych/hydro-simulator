#ifndef STREAMCONNECTOR_H
#define STREAMCONNECTOR_H

#include <QFuture>
#include <QObject>
#include <queue>
#include <QUdpSocket>
#include <mutex>
#include <QVariantMap>
#include "Protocol.h"
#include "TCPWorkers_global.h"

typedef std::function<void (const QVariant &)> token;
typedef std::shared_ptr<token> listener;

struct TCPWORKERS_EXPORT StreamHandler {
//    Q_OBJECT
//public:
//    enum Mode {Sender = 0, Receiver};
    std::string name;
    QUdpSocket * socket;
    int port;
    QVector<std::weak_ptr<token>> handlers;
    explicit StreamHandler(){
        qRegisterMetaType<QVector<double>>("QVector<double>");
    }
};
//void UdpWorker::process()
//{
//    if(file_list.count() == 0) {
//        emit finished();
//        return;
//    }
//    switch (r_type) {
//        case REPORT_A: {
//            rb            = new ReportBuilderA ();
//            break;
//        }
//        case REPORT_B: {
//            rb            = new ReportBuilderB ();
//            break;
//        }
//        case REPORT_C: {
//            rb            = new ReportBuilderC ();
//            break;
//        }
//        default:
//            emit finished();
//            return ;
//        }
//    }

//    rb->buildToFile(file_list); /* выполнение  buildToFile прерывается вызовом rb->stop() */
//    emit finished();
//    return ;
//}

//void UdpWorker::stop() {
//    socket
//    return ;
//}


//!
//! \brief The StreamConnector singleton class
//!
class TCPWORKERS_EXPORT StreamConnector : public QObject
{
    Q_OBJECT
    static StreamConnector * pinstance_;
    static std::mutex mutex_;
//    K_THREADSAFE_SINGLETON(StreamConnector)
    static std::mutex handlerMutex;

    bool server = false;

public:
    explicit StreamConnector(QObject *parent = nullptr);

    //!
    //! \brief Returns instance for one thread and blocks it
    //! \return
    //!
    static StreamConnector * instance() {
        std::lock_guard<std::mutex> lock(mutex_);
            if (pinstance_ == nullptr)
            {
                pinstance_ = new StreamConnector;
            }
            return pinstance_;
    }
    ~StreamConnector();

    //!
    //! \brief Write data request for adding stream on client side
    //! \param name
    //! \return request binary data
    //!
    QByteArray writeRequestAdd(std::string name);

    //!
    //! \brief Returns unique bind names for request on client side
    //! \return
    //!
    QVector<QString> uniqueBindNames();

    StreamConnector(StreamConnector &other) = delete;

    void operator=(const StreamConnector &) = delete;
    void setServer(bool value);

    //!
    //! \brief Adds stream sender object (server side)
    //! \param name
    //! \param sender
    //!
    void addStreamSender(const std::string & name, std::function<void(const QVariant&)> sender = nullptr);

signals:
    void streamModeChanged(bool value);
public slots:

    //!
    //! \brief Sends \a data into stream by given \a name
    //! \param data
    //! \param name
    //!
    void sendData(const QVariant &data, const std::string &name);


    //!
    //! \brief Adds stream receiver (client side)
    //! \param port
    //! \param name
    //!
    void addStreamReceiver(int port, const std::string &name); // for sending

    //!
    //! \brief Binds to stream by given \a name and callback, which is executed when new data arrives to stream
    //! \param name
    //! \param onReceived
    //!
    std::shared_ptr<token> bindToStream(const std::string & name, token onReceived);

    //!
    //! \brief Removes stream by given \a sock pointer
    //! \param sock
    //!
    void removeStream(QUdpSocket * sock);

    //!
    //! \brief Slot function which adds stream on client side when server confirmed
    //! \param stream
    //!
    void onStreamAnotherSideAdded(QDataStream * stream);

//    void pushToStreamServer(std::string where, const QVariant & value);

//    void unbindFromStream(QString, std::function<void>)

    void clearHandlers();
private:
    const int startPort = 44450;
    const int mainPort = 44449;
    int currentPort = startPort;
    std::vector<int> availablePorts;
    QMap<QUdpSocket*, StreamHandler*> streamSockets;
    QMap<std::string, StreamHandler*> handlersNames;
    std::queue<QPair<QUdpSocket*, StreamHandler*>> requestQueue;
    std::queue<QPair<QString, std::weak_ptr<token>>> bindQueue;
    QVector <QString> uniqueNames;
    QHostAddress groupAddress;
    QUdpSocket * mainServerSocket = nullptr;

    QVector<QFuture<void>> futures;



private slots:
    //!
    //! \brief onReadyRead
    //!
    void onReadyRead();

};

#endif // STREAMCONNECTOR_H
