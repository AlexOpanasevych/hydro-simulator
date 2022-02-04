#ifndef SERVERSERVICE_H
#define SERVERSERVICE_H

#include "client.h"
#include "station.h"
#include <tcpserverworker.h>

#include <QUdpSocket>
#include <QByteArray>
#include <QObject>
#include <kmacro.h>
#include <QTcpServer>
#include <QTimer>

#include <utility/kobservablelist.h>

#include <src/stations/bronze.h>
#include <src/stations/platina.h>

class ServerConfigurator;
class Resource;

class SERVERINTERFACE_LIBRARY_EXPORT ServerService : public QObject
{
    Q_OBJECT
    K_QML_TYPE(ServerService)


    K_AUTO_PROPERTY(
            KObservableModel*,
            clientsModel,
            clientsModel,
            setClientsModel,
            clientsModelChanged,
            nullptr)

    K_AUTO_PROPERTY(
            KObservableModel*,
            modesModel,
            modesModel,
            setModesModel,
            modesModelChanged,
            nullptr)

    K_AUTO_PROPERTY(Bronze*, bronzeStation, bronzeStation, setBronzeStation, bronzeStationChanged, nullptr)
    K_AUTO_PROPERTY(Platina*, platinaStation, platinaStation, setPlatinaStation, platinaStationChanged, nullptr)

public:
    friend ServerConfigurator;
    friend Resource;

//    explicit ServerService(QObject * parent = nullptr);
    explicit ServerService(QObject * parent = nullptr);
    ~ServerService();

    //!
    //! \brief Sends resources to client
    //! \param socketDescriptor
    //!
    inline void sendResources(int socketDescriptor);

public slots:

    //!
    //! \brief When new connection is performed, searches for client in clients list (if not present, then adds to list)
    //!
    void newConnection( QHostAddress, int );

    //!
    //! \brief If client is alive, removes client from list
    //! \param socketDescriptor
    //!
    void removeClient( int socketDescriptor );


    void updateStates() {m_bronzeStation->initializeResources(); m_platinaStation->initializeResources();}
private:
    int countOfClients = 0;

    int lastSendedId = 0;

    QList<QVariantMap> currentClientQml;

    const QVector<QString> bronzeNames {"bronze1", "bronze2"};
    const QVector<QString> platinaNames {"platina1", "platina2", "platina3", "platina4", "platina5"};

protected:
    KObservableList<Client> clientsList;

    TcpServerWorker worker;

    KObservableList<Mode> modeList;
};

#endif // SERVER_H
