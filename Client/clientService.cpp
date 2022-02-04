#include "clientservice.h"
#include "clientsideparser.h"
#include "mgkitemsfactory.h"
#include "MessageType.h"
#include "simdatamanager.h"

#include <QtConcurrent/QtConcurrent>

#include <QSysInfo>

#define RECEIVE_PORT 45454
#define SEND_PORT 45455

//#define DISABLE_SERVER_CONNECT

ClientService::ClientService(QObject *parent) : QObject(parent), worker("127.0.0.1", 33333, false), playTimer(this)
{
    m_itemsModel = itemsList.model();
    m_itemsModel->setParent(this);
    reconnectTimer = new QTimer(this);

#ifndef DISABLE_SERVER_CONNECT

    using namespace std::placeholders;
    worker.setOnConnectionPerformed(std::bind(&ClientService::connect, this));
    worker.setReceiver(std::bind(&ClientService::messageHandler, this, _1));

    QObject::connect(reconnectTimer, &QTimer::timeout, this, [this](){
        if(worker.connectToServer())
            reconnectTimer->stop();
    });
    QObject::connect(worker.socket(), &QTcpSocket::disconnected, this, &ClientService::serverDisconnected);
    QObject::connect(worker.socket(), &QTcpSocket::connected, this, &ClientService::serverConnected);
    QObject::connect(worker.socket(), &QTcpSocket::disconnected, this, [this] () {
        itemsList.clear();
        StreamConnector::instance()->clearHandlers();
//        qDebug() << "cleared" << itemsList.count();
        reconnectTimer->start(500);
    });
//    QObject::connect(device.get(), &QIODevice::readyRead, this, [](){qDebug() << "ready read!!!!!!!!!!!!!!!!!!!!!!!!!";});

//    QObject::connect(&playTimer, &QTimer::timeout, this, [this](){
//        device->write(buffer.data(), buffer.size());
//        qDebug() << "write";
//    });


    reconnectTimer->start(500);
#endif

    QAudioDeviceInfo deviceInfo = QAudioDeviceInfo::defaultOutputDevice();

    m_audio_lib = new AudioStreamingLibCore(this);

    AudioStreamingLibInfo info;

    info.setWorkMode(AudioStreamingLibInfo::StreamingWorkMode::BroadcastClient);
    info.setOutputDeviceInfo(deviceInfo);
    info.setTimeToBuffer(0);
    info.setEncryptionEnabled(false);
    info.setGetAudioEnabled(true);
    info.setNegotiationString("audioStream");

    m_audio_lib->start(info);

    m_audio_lib->setVolume(QAudio::convertVolume(m_volume / 100., QAudio::LogarithmicVolumeScale, QAudio::LinearVolumeScale));

    m_audio_lib->connectToHost("192.168.81.161", quint16(45454));

}

ClientService::~ClientService()
{

}

void ClientService::reconnect()
{
    reconnectTimer->start(500);
}

void ClientService::changeVolume(int value)
{
//    setVolume(value);
//    auto volumeOutput = QAudio::convertVolume(qBound(0., value / 100., 1.), QAudio::LogarithmicVolumeScale,
//                                              QAudio::LinearVolumeScale);
//    m_audioOutput->setVolume(volumeOutput);
    m_audio_lib->setVolume(value);
}

void ClientService::connectedToAudioClient(const QHostAddress &address, const QString &id)
{
    Q_UNUSED(address)
    Q_UNUSED(id)
    if(!m_audio_lib->isRunning()) m_audio_lib->start(AudioStreamingLibInfo());
//    QString peer = !id.isEmpty() ? id : QHostAddress(address.toIPv4Address()).toString();

//    QString title = QString("Connected to: %0 - %1").arg(peer).arg(TITLE);

//    setWindowTitle(title);

//    boxautostart->setEnabled(false);

//    if (boxautostart->isChecked())
//        startPauseRecord(); //Auto start recording when connected
}

void ClientService::disconnectedFromAudioClient(const QHostAddress &address)
{
    Q_UNUSED(address)

//    stopRecord();
//    resetRecordPage();

    if (m_audio_lib)
        m_audio_lib->stop();

//    boxautostart->setEnabled(true);

//    setWindowTitle(TITLE);
}

void ClientService::messageHandler(const Frame & frame)
{
    // receive id
    if(!connectStatus) {
        QDataStream s(frame.data);
        quint8 typeOfData;
        s >> typeOfData;
        if(typeOfData == CONNECT_ID_MARK) {
            QString id;
            s >> id;
            // this is valid id
            if(m_id == id) {
                connectStatus = true;
                qDebug() << "connected";

            }


        }
    }
    else {
        QDataStream s(frame.data);
        s.setVersion(QDataStream::Qt_5_15);
        uint8_t typeOfData;
        s >> typeOfData;

        switch (typeOfData) {

        case UNLOAD_DATA: {
//            emit clearQMLData();
            itemsList.clear();
            StreamConnector::instance()->clearHandlers();
            break;
        }


        case RECEIVE_BY_ID: {
//            qDebug() << "received data";
            //emit recvDataFromServer();
            uint8_t deliveryType;
            s >> deliveryType;
            switch(deliveryType){
            case QML_AS_STRING: {
                ClientSideParser cParser(deliveryType);
                cParser.parse(&s);
                auto result = cParser.captured();
                MGKItem * newItem = new MGKItem(this);/*MGKItemsFactory::produceItem(result.first().toMap()["globalControlType"].toString());*/
                newItem->init(result.first().toMap());
                itemsList.push_back(newItem);

                break;
            }
            case QML_AS_ZIP: {

                QByteArray data;
                s >> data;

                zipBuffer.append(data);
                // next we can proceed qml or info
                break;
            }
            }
            break;
        }

        case END_RECEIVE_BY_ID: {

            uint8_t typeOfDelivery;
            s >> typeOfDelivery;

            switch (typeOfDelivery) {
            case QML_AS_STRING: {
//                qDebug() << "model constructed";
                emit itemsModelChanged(m_itemsModel);
                // request for creating streams
                Frame requestFrame;
                QDataStream s(&requestFrame.data, QIODevice::WriteOnly);
                auto names = StreamConnector::instance()->uniqueBindNames();
//                qDebug() << "sefsfsf" << names;
                s << ADD_STREAM << names.size();

                for(auto && name : names) {
                    s << name;
                }

                worker.writeData(requestFrame);
                break;
            }
            case QML_AS_ZIP: {
//                qDebug() << "buffer constructed";
                ClientSideParser parser(typeOfDelivery);
                parser.parse(&s);
                QByteArray formData;
                zipBuffer.clear();
                break;
            }
            }
            break;
        }

        case ADD_STREAM: {
            int port = 0;
            QString name;
            s >> port >> name;
            StreamConnector::instance()->addStreamReceiver(port, name.toStdString());
            break;
        }

        case INIT_RECEIVE: {

                uint8_t typeOfDelivery;
                s >> typeOfDelivery;
                switch(typeOfDelivery) {
                case QML_AS_STRING: {
                    itemsList.clear();
                    QString clName;
                    s >> clName;
                    setClientName(clName);
                    break;
                }
                case QML_AS_ZIP: {
                    zipBuffer.clear();
                    break;
                }
                }
            break;
        }

        case STATE_CHANGED: {
            int stateId ;
            QVariant value;
            QString clientName;
            s >> stateId;
            s >> clientName;
            s >> value;
            m_sCommunicator->receiveFromServer(stateId, value);
            break;
        }

        case LOAD_STATES: {
            QVariantList stateDataList;
            s >> stateDataList;
            for(const auto &item : qAsConst(stateDataList)) {
                auto map = item.toMap();
                auto stateid = map["stateId"].toInt();
                auto value = map["value"];
                m_sCommunicator->receiveFromServer(stateid, value);
            }
            break;
        }

        case SET_SIMDATA: {
            qDebug() << "simData arrived";
            QVariantMap data;
            s >> data;
            SimDataManager::instance()->fromMap(data);
            break;
        }

        default:
            qDebug() << "this is not expected to happen";
            break;
        }

    }
}

void ClientService::connect()
{
    Frame frame;
    m_id = QSysInfo::machineUniqueId();
    QDataStream s(&frame.data, QIODevice::WriteOnly);
    s << CONNECT_ID_MARK << m_id;
    worker.writeData(frame);

    frame.data.clear();
    s.device()->reset();

    QDir dir("./resources");
    if(!dir.exists()) {
        s << LOAD_ZIP_REQUEST;
        worker.writeData(frame);
    }


//    return allGood;
}
