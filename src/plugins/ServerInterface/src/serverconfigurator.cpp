#include "serverconfigurator.h"
#include "mode.h"

#include <cdebug.h>
#include <src/MessageType.h>
#include <QJsonDocument>
#include <QDir>
#include <QDirIterator>
#include <QTextStream>

#ifdef Q_NO_DEBUG
ServerConfigurator::ServerConfigurator(ServerService* sService, QString appPath, QObject *parent) : QObject(parent), dbProvider(appPath + "/src/plugins/ServerInterface/QML.db")
#else
ServerConfigurator::ServerConfigurator(ServerService* sService, QString appPath, QObject *parent) : QObject(parent), dbProvider(appPath + "/src/plugins/ServerInterface/QML.db")
#endif
{
    service = sService;
    dbProvider.setTable("QML");
    loadModes();
    loadClients();

    for(auto && client : service->clientsList) {
        connect(client, &Client::connectedChanged, this, [this](){
            Client * client = qobject_cast<Client*>(sender());
            if(client->connected()) {
                if(client->mode() != nullptr) {
                    loadData(client->mode()->name(), client->socketDescriptor());
                }
            }
        });
    }
}

ServerConfigurator::~ServerConfigurator()
{
    qDebug(s) << "saving modes and clients";
    saveModes();
    saveClients();
}

void ServerConfigurator::bindClientToMode(Client *client)
{
    if(client == nullptr || m_enteredMode == nullptr) {
        qDebug(s) << "nothing sssssssssssssssssssssssssssssssssss";
        return;
    }

    if(client->mode() != nullptr) {
        client->mode()->removeClient(client);
    }

    client->setMode(enteredMode());
    enteredMode()->addClient(client);

    service->worker.setMapping(client->socketDescriptor(), enteredMode()->name());

    loadData(enteredMode()->name(), client->socketDescriptor());
    //sendResources(client->absoluteId(), client->socketDescriptor());
}

void ServerConfigurator::unbindClientFromMode(Client * client)
{
    if(client != nullptr) {
        unloadData(client->socketDescriptor());
        client->mode()->clientList().remove(client);
        emit service->modesModelChanged(service->m_modesModel);
        client->setMode(nullptr);
    }
}

void ServerConfigurator::loadModes()
{
    dbProvider.setTable("QML_copy");
    auto result = dbProvider.select().get();

    for(auto && item : result) {
        Mode * mode = new Mode(this);
        mode->setX(item["x"].toInt());
        mode->setY(item["y"].toInt());
        mode->setName(item["name"].toString());
        mode->setWidth(item["width"].toInt());
        mode->setHeight(item["height"].toInt());
        service->modeList.push_back(mode);
        emit service->modesModelChanged(service->m_modesModel);
    }
}

void ServerConfigurator::saveModes()
{
    dbProvider.setTable("QML_copy");
    for (auto mode = service->modeList.cbegin(); mode != service->modeList.cend(); mode++) {
        dbProvider.updateRow({{"x", (*mode)->x()}, {"y", (*mode)->y()}, {"width", (*mode)->width()}, {"height", (*mode)->height()}}, {"name", (*mode)->name()});
    }
}

void ServerConfigurator::saveClients()
{
    if(service->clientsList.count() > 0) {
        dbProvider.setTable("clients");
        for(auto && client : service->clientsList) {
            QString mode = client->mode() != nullptr ? client->mode()->name() : "";
            dbProvider.updateInsert({{"addr", client->addr()}}, {{"addr", client->addr()}, {"port", client->port()}, {"socketDescriptor", client->socketDescriptor()}, {"modeName", mode}, {"uid", client->uid()}, {"absoluteId", client->absoluteId()}}, {{"addr", client->addr()}, {"port", client->port()}, {"socketDescriptor", client->socketDescriptor()}, {"modeName", mode}, {"uid", client->uid()}, {"absoluteId", client->absoluteId()}},
            {{"addr", client->addr()}});
        }

    }
}

//IF NOT EXISTS(SELECT * FROM [dbo].[clients] WHERE (addr = 1) ) BEGIN INSERT INTO [dbo].[clients] (absoluteId, addr, modeName, port, socketDescriptor, uid) VALUES(1, '1', '', 1, 1, '1') END ELSE BEGIN UPDATE [dbo].[clients] SET absoluteId = 1, addr = '1', modeName = '', port = 1, socketDescriptor = 1, uid = '' WHERE (addr = '1')  END

void ServerConfigurator::loadClients()
{
    if(!service->modeList.count()) {
        loadModes();
    }

    dbProvider.setTable("clients");
    auto result = dbProvider.select().get();
    qDebug(s) << "attempt to add client" << result;
    for(auto && item : result) {
        Client * client = new Client(service);
        client->setUid(item["uid"].toString());
        client->setAddr(item["addr"].toString());

        for(auto && mode : service->modeList) {
            if(mode->name() == item["modeName"].toString()) {
                client->setMode(mode);
                mode->addClient(client);
                qDebug(s) << "adding client to mode" << mode->name();
                break;
            }
        }
        client->setPort(item["port"].toInt());
        client->setAbsoluteId(item["absoluteId"].toInt());
        service->clientsList.push_back(client);
        emit service->clientsModelChanged(service->m_clientsModel);
    }

}

void ServerConfigurator::deleteAllClients()
{
    dbProvider.setTable("clients");
    dbProvider.del().get();
    service->clientsList.clear();
}

void ServerConfigurator::sendSimulationData(const QVariantMap &data)
{
    QByteArray arr;
    QDataStream s(&arr, QIODevice::WriteOnly);
    s << SET_SIMDATA << data;
    qDebug() << "sending simData" << arr.toHex('.');
    for(auto & c : service->clientsList)
        if(c->mode())
            service->worker.sendDataByID(c->mode()->name(), arr);
}


void ServerConfigurator::unloadData(int socketDescriptor)
{
    Frame requestFrame;
    requestFrame.type = SendType::P2P;
    requestFrame.socketDescriptor = socketDescriptor;
    QDataStream s(&requestFrame.data, QIODevice::WriteOnly);

    s << DATA_UNLOAD;

    service->worker.writeData(requestFrame);
}

void ServerConfigurator::loadData(QString name, int socketDescriptor)
{
    dbProvider.setTable("QML");
    QList<QVariantMap> qml;
    if(name != "all")
        qml = dbProvider.select().where({"name", name}).get();
    else qml = dbProvider.select().get();

    proceedAdditionalQml(qml);

    Frame requestFrame;
    requestFrame.type = SendType::P2P;
    requestFrame.socketDescriptor = socketDescriptor;
    QDataStream s(&requestFrame.data, QIODevice::WriteOnly);

    s << START_SEND_BY_ID << QML_STRING << name;

    service->worker.writeData(requestFrame);

    for(auto && item : qml) {
        requestFrame.data.clear();
        s.device()->reset();
        s.setVersion(QDataStream::Qt_5_15);
        QJsonDocument jsonMap = QJsonDocument::fromVariant(item);
        s << SEND_BY_ID << QML_STRING << jsonMap;
        service->worker.writeData(requestFrame);
    }


    requestFrame.data.clear();
    s.device()->reset();
    s << END_SEND_BY_ID << QML_STRING;
//                    m_negotiator.writeFrame((uint8_t*)packet.data(, packet.size());
    service->worker.writeData(requestFrame);

    requestFrame.data.clear();
    s.device()->reset();
    if(name != "all")
        s << LOAD_STATES << (name.chopped(1) == service->m_platinaStation->name() ? service->m_platinaStation->getStateDataAsMap(name) : service->m_bronzeStation->getStateDataAsMap(name));
    else {
        for(auto && it = service->modeList.begin(); it != service->modeList.end(); it++){
            s << LOAD_STATES << ((*it)->name().chopped(1) == service->m_platinaStation->name() ? service->m_platinaStation->getStateDataAsMap(name) : service->m_bronzeStation->getStateDataAsMap(name));
        }
    }
    service->worker.writeData(requestFrame);
    return;


}

void ServerConfigurator::proceedAdditionalQml(QList<QVariantMap> &qml)
{
//    QVector<int> ids;
    for(auto && item : qml) {
//        ids.append(item["id"].toInt());
        auto & additionalQml = item["additionalQml"];
        QVariantMap aQml;

        auto splitted = additionalQml.toString().split(";", Qt::SkipEmptyParts);
        for(auto && property : splitted) {
            auto spl = property.split(":", Qt::SkipEmptyParts);
            auto array = spl[1].split(",", Qt::SkipEmptyParts);
            QVariant value;
            QVariantList list;

            if(array.size() > 1) {
                for(auto && elem: array) {
                    getValueFromString(elem, value);
                    list << value;
                }
                value = list;
            }
            else
            {
                value = spl[1];
            }
            if(spl.size() == 2) aQml.insert(spl[0], value);

        }

//        qDebug() << item["additionalQml"].toString().replace(":", "::");
//        dbProvider.updateRow({{"additionalQml", item["additionalQml"]}}, {"additionalQml", item["additionalQml"].toString().replace(":", "::")});
        item["additionalQml"] = aQml;
    }


}

void getValueFromString(QString str, QVariant& returnVal)
{
    bool ok;
    int tryValueI;
    double tryValueD;
    tryValueI = str.toInt(&ok);
    if(ok) {
        returnVal = tryValueI;
    }
    else {
        tryValueD = str.toDouble(&ok);
        if(ok) {
            returnVal = tryValueD;
        }
        else returnVal = str;
    }
}
