#include "serverservice.h"

#include <QDataStream>
#include <QtConcurrent/QtConcurrent>
#include <QFile>
#include <QTcpSocket>
#include <QVariantMap>
#include <src/MessageType.h>
#include <src/qzipwriter_p.h>
#include "state.h"
#include "mode.h"
#include <streamconnector.h>
#include <Protocol.h>
#include <cdebug.h>

ServerService::ServerService(QObject * parent) : QObject(parent), m_bronzeStation(new Bronze(this)), m_platinaStation(new Platina(this)), worker("127.0.0.1", 33333)
{
    m_bronzeStation->setService(this);
    m_platinaStation->setService(this);
//    sConf
    connect(&worker, SIGNAL(newConnection(QHostAddress, int)), this, SLOT(newConnection(QHostAddress, int)));
    connect(&worker, SIGNAL(connectionRemoved(int)), this, SLOT(removeClient(int)));

//    connect(static_cast<Transmitter*>(station->subsysList()[1])->module(), QOverload<int>::of(&AudioModule::initVolumeChanged), this, QOverload<int>::of(&ServerService::initVolumeChanged));
    setClientsModel(clientsList.model());
    setModesModel(modeList.model());
    m_modesModel->setParent(this);
    m_clientsModel->setParent(this);

//    connect(StreamConnector::instance(), QOverload<int, StreamHandler::Mode, QString>::of(&StreamConnector::requestAddStream), this, [this](int port, StreamHandler::Mode mode, QString name){
//        qDebug() << "sending request";
//        Frame frame;
//        frame.data = StreamConnector::instance()->writeRequestAdd(port, mode, name);
//        frame.type = SendType::P2P;
//        worker.writeData(frame);
//    });
    StreamConnector::instance()->addStreamSender("dopler");
    StreamConnector::instance()->addStreamSender("ep1sweep");

    m_platinaStation->setSendCallback([this](const QList<StateData>& stateData){
//        qDebug() << "exec from another thread platina";
        for(auto & state : stateData) {
            Frame requestFrame;
            requestFrame.type = SendType::Broadcast;
            requestFrame.socketDescriptor = state.socketDescriptor;

            QDataStream ans(&requestFrame.data, QIODevice::WriteOnly);
            ans << STATE_CHANGED
                << state.stateID
                << state.clientName
                << state.value;
            qDebug() << state.value;
            worker.writeData(requestFrame);
        }
    });

    m_bronzeStation->setSendCallback([this](const QList<StateData>& stateData){
//        qDebug() << "exec from another thread bronze";s
        for(auto && state : stateData) {
//            Frame requestFrame;
//            requestFrame.type = SendType::Broadcast;
//            requestFrame.socketDescriptor = state.socketDescriptor;
            QByteArray data;

            QDataStream ans(&data, QIODevice::WriteOnly);
            ans << STATE_CHANGED
                << state.stateID
                << state.clientName
                << state.value;
            qDebug() << data.toHex('.');
//            worker.writeData(requestFrame);
            worker.sendDataByID(state.clientName, data);
        }
    });

    worker.setReceiver([this](const Frame& frame){
        QDataStream ss(frame.data);
        quint8 typeOfData;
        ss >> typeOfData;
        qDebug(s) << "adadadadad";
        switch(typeOfData) {
        case REQUEST_STATE_CHANGE: {

            StateData state;

            ss >> state.stationName >> state.stateID >> state.resourceName >> state.clientName >> state.value;

            state.socketDescriptor = frame.socketDescriptor;

            qDebug(s) << state.stationName;

            if(state.stationName == "platina") {
                qDebug(s) << "pl station";
                m_platinaStation->pushState(state);
            }  else {
                qDebug(s) << "bronze station";
                m_bronzeStation->pushState(state);
            }

            break;

        }

        case ADD_STREAM: {
            int size;
            ss >> size;
            qDebug(s) << size;
            for(int i = 0; i < size; i++) {
                QString name;
                ss >> name;
                qDebug(s) << name;
                Frame requestFrame;
                requestFrame.data = StreamConnector::instance()->writeRequestAdd(name.toStdString());
                requestFrame.type = SendType::P2P;
                requestFrame.socketDescriptor = frame.socketDescriptor;
                worker.writeData(requestFrame);
            }
            //StreamConnector::instance()->onStreamAnotherSideAdded(&s);
            break;
        }
        case ZIP_UPLOAD: {
            sendResources(frame.socketDescriptor);
            break;
        }
        case CONNECTION_TYPE: {
            QString id;
            ss >> id;
            bool create = true;
            if(clientsList.count()) {
                for(auto && client : clientsList) {
                    if(client->uid() == id) {
                        create = false;
                        Frame requestFrame;
                        //QDataStream ans(&requestFrame.data, QIODevice::WriteOnly);
                        QDataStream ans(&requestFrame.data, QIODevice::WriteOnly);
                        ans << CONNECTION_TYPE << id;
                        requestFrame.type = SendType::P2P;
                        requestFrame.socketDescriptor = frame.socketDescriptor;
                        qDebug(s) << frame.socketDescriptor;
                        worker.writeData(requestFrame);
                        client->setConnected(true);

                        if(client->mode())
                            worker.setMapping(frame.socketDescriptor, client->mode()->name());

                        qDebug(s) << "connected to" << frame.socketDescriptor << "id:" << id;
                    }
                }
            }

            if(create) {

                clientsList.last()->setUid(id);
                Frame requestFrame;
                //QDataStream ans(&requestFrame.data, QIODevice::WriteOnly);
                QDataStream ans(&requestFrame.data, QIODevice::WriteOnly);
                ans << CONNECTION_TYPE << id;
                requestFrame.type = SendType::P2P;
                requestFrame.socketDescriptor = clientsList.last()->socketDescriptor();
                worker.writeData(requestFrame);
                qDebug(s) << "connected to" << clientsList.last()->socketDescriptor() << "id:" << id;
            }

            break;
        }
        default:
            qDebug() << "this is not expected to happen";
            break;
        }
    });

}

ServerService::~ServerService()
{
}

void ServerService::newConnection(QHostAddress addr, int port)
{
    bool create = true;
    for(auto && client : clientsList) {
        if(client->addr() == addr.toString()) {
            qDebug() << "client exists, bind to new socket and setting alive";
            create = false;
            client->setSocketDescriptor(worker.getSocketByAddr(addr.toString())->socketDescriptor());
            client->setPort(port);
            client->setAlive(true);
        }
    }
    if(create) {
        Client * newClient = new Client(this);
        newClient->setAddr(addr.toString());
        newClient->setPort(port);
        newClient->setAbsoluteId(clientsList.count());
        newClient->setAlive(true);
        newClient->setSocketDescriptor(worker.socket(worker.clientCount() - 1)->socketDescriptor());
        clientsList.push_back(newClient);

        emit clientsModelChanged(m_clientsModel);
    }
}

void ServerService::removeClient(int socketDescriptor)
{
    for(auto && client : clientsList) {
        if(client->isAlive() && client->socketDescriptor() == socketDescriptor) {
            client->setAlive(false);
            client->setConnected(false);
        }
    }
}

void ServerService::sendResources(int socketDescriptor)
{
    //send files as zip archive
    QDir dir(".");
    if(!dir.exists("images.zip")) {
        QZipWriter writer("images.zip");
        if (writer.status() != QZipWriter::NoError) {
            qDebug() << "shit happened";
            return;
        }

        writer.setCompressionPolicy(QZipWriter::AutoCompress);

        QString path = QLatin1String("../../resourses/images/");
        QDirIterator it(path, QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot,
                        QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString file_path = it.next();
            qDebug() << file_path;
            if (it.fileInfo().isDir()) {
                writer.setCreationPermissions(QFile::permissions(file_path));
                writer.addDirectory(file_path.remove(path));
            } else if (it.fileInfo().isFile()) {
                QFile file(file_path);
                if (!file.open(QIODevice::ReadOnly))
                    continue;

                writer.setCreationPermissions(QFile::permissions(file_path));
                QByteArray ba = file.readAll();
                writer.addFile(file_path.remove(path), ba);
            }
        }
        writer.close();
    }

    QFile arch("images.zip");
    arch.open(QFile::ReadOnly);

    QByteArray packet;
    QDataStream ss(&packet, QIODevice::WriteOnly);

    ss << START_SEND_BY_ID << QML_ZIP;
    Frame frame;
    frame.data = packet;
    frame.type = SendType::P2P;
    frame.socketDescriptor = socketDescriptor;
    worker.writeData(frame);
    packet.clear();
    ss.device()->reset();
    ss << SEND_BY_ID << QML_ZIP << QString("images/") << arch.readAll();
    frame.data = packet;
    frame.type = SendType::P2P;
    frame.socketDescriptor = socketDescriptor;
    worker.writeData(frame);
    //m_negotiator.writeFrame((uint8_t*)packet.data(), packet.size());
    //    for(int i = 0; i < countOfPackets; i++) {
    //        packet.clear();
    //        s.device()->reset();
    //        s << SEND_BY_ID << connectedClients.indexOf(clientUID) + 1 << QML_ZIP << arch.read(200);
    //        transmitQueue.enqueue(packet);
    //    }
    packet.clear();
    ss.device()->reset();

    ss << END_SEND_BY_ID << QML_ZIP;
    frame.data = packet;
    frame.type = SendType::P2P;
    frame.socketDescriptor = socketDescriptor;
    worker.writeData(frame);
    qDebug(s) << "loaded resources";
    //m_negotiator.writeFrame((uint8_t*)packet.constData(), packet.size());
}
