#ifndef CLIENTSERVICE_H
#define CLIENTSERVICE_H


#include <QElapsedTimer>
#include <QFuture>
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <QUdpSocket>
#include <kmacro.h>
#include <tcpclientworker.h>
#include <utility/kobservablelist.h>
#include <utility/sounddevice.h>
#include <audiostreaminglibcore.h>
#include <QAudioOutput>
#include <streamconnector.h>
#include "mgkitembase.h"
#include "statecommunicator.h"
class ClientService : public QObject
{
    Q_OBJECT
    K_QML_TYPE(ClientService)
    K_READONLY_PROPERTY( QString, id, id, setId, idChanged, "")
    K_READONLY_PROPERTY( QString, clientName, clientName, setClientName, clientNameChanged, "")
    K_AUTO_PROPERTY(KObservableModel*, itemsModel, itemsModel, setItemsModel, itemsModelChanged, nullptr)
    K_AUTO_PROPERTY(int, volume, volume, setVolume, volumeChanged, 50)
    K_READONLY_PROPERTY(StateCommunicator*, sCommunicator, sCommunicator, setSCommunicator, sCommunicatorChanged, new StateCommunicator(this, this))
public:
    friend StateCommunicator;
    explicit ClientService(QObject *parent = nullptr);
    ~ClientService();
signals:

    void stateChanged(int index, int state);
    void serverDisconnected();
    void serverConnected();
public slots:
    void connect();
    //void readSocket();
    //void discardSocket();
    void messageHandler(const Frame &);
//    void sendState(int index, int state, double value);
    void reconnect();

    void changeVolume(int value);
    void connectedToAudioClient(const QHostAddress &address, const QString &id);
    void disconnectedFromAudioClient(const QHostAddress &address);

private:
    void load();

    KObservableList<MGKItem> itemsList;

    bool status = true;
    //bool m_running = true;
    bool connectStatus = false;
    QFuture<void> m_future;

    QByteArray zipBuffer;
    QPointer<AudioStreamingLibCore> m_audio_lib;
    QPointer<AudioStreamingLibCore> m_discover_instance;
protected:
    TcpClientWorker worker;

    QSharedPointer<QAudioOutput> m_audioOutput;
    QSharedPointer<QIODevice> device;
    QTimer * reconnectTimer = nullptr;
    QTimer playTimer;
    QByteArray buffer;
};

#endif // CLIENTSERVICE_H
