#ifndef SERVERCONFIGURATOR_H
#define SERVERCONFIGURATOR_H

#include <sqliteprovider.h>
#include <ssmsprovider.h>
#include <QObject>
#include "serverservice.h"

class SERVERINTERFACE_LIBRARY_EXPORT ServerConfigurator : public QObject
{
    Q_OBJECT
    K_AUTO_PROPERTY(Mode *, enteredMode, enteredMode, setEnteredMode, enteredModeChanged, nullptr)
    K_QML_TYPE(ServerConfigurator)
public:
    explicit ServerConfigurator(ServerService * sService = nullptr, QString appPath = "", QObject *parent = nullptr);
    ~ServerConfigurator();

    //!
    //! \brief Proceed additional QML
    //! \param qml
    //!
    inline void proceedAdditionalQml(QList<QVariantMap> & qml);

    //!
    //! \brief Loads data from database
    //! \param name
    //! \param socketDescriptor
    //!
    inline void loadData(QString name, int socketDescriptor);
    //!
    //! \brief Sends unload data request
    //! \param socketDescriptor
    //!
    inline void unloadData(int socketDescriptor);
public slots:

    //!
    //! \brief Binds client to current selected mode
    //! \param client
    //!
    void bindClientToMode(Client *client);
    //!
    //! \brief Unbinds client from bound mode
    //! \param client
    //!
    void unbindClientFromMode(Client *client);

    //!
    //! \brief Loads modes from DB
    //!
    void loadModes();

    //!
    //! \brief Saves modes data to DB
    //!
    void saveModes();

    //!
    //! \brief Saves clients data to DB
    //!
    void saveClients();

    //!
    //! \brief Loads clients data from DB
    //!
    void loadClients();

    //!
    //! \brief Deletes all clients from DB
    //!
    void deleteAllClients();

    void sendSimulationData(const QVariantMap& data);
private:
    ServerService* service = nullptr;
    SqliteProvider dbProvider;
};

//!
//! \brief Convert value in string to QVariant
//! \param str
//! \param returnVal
//!
void getValueFromString(QString str, QVariant& returnVal);

#endif // SERVERCONFIGURATOR_H
