#ifndef STATION_H
#define STATION_H

#include "src/abstractsubsystem.h"

#include "state.h"

//#include <QFuture>
//#include <QFutureWatcher>
#include <sqliteprovider.h>
#include <kcirclebuffer.h>
#include <QMutex>
#include <utility/kobservablelist.h>
class ServerService;
class AbstractSubsystem;
class MGK345Wrapper;

class SERVERINTERFACE_LIBRARY_EXPORT Station : public QObject
{
    Q_OBJECT
    K_QML_INTERFACE(Station)
    std::vector<AbstractSubsystem *> m_subsysList;
    QString m_name;
    QMutex m;
    K_READONLY_PROPERTY(KObservableModel*, stateModel, stateModel, setStateModel, stateModelChanged, nullptr)
    K_AUTO_PROPERTY(QString, currentClient, currentClient, setCurrentClient, currentClientChanged, "")
protected:
    KObservableList<Resource> m_statesList;

    friend AbstractSubsystem;
    friend MGK345Wrapper;
public:
    Station(QString name, QObject * parent = nullptr);
    ~Station();

    //!
    //! \brief Gets subsystem list
    //! \return subsystem list
    //!
    std::vector<AbstractSubsystem*> subsysList() const;

    void addSubsystem(AbstractSubsystem* s);

//    //!
//    //! \brief Gets states list
//    //! \return states list
//    //!
//    std::vector<Resource*> statesList() const;

//    //!
//    //! \brief Sets new states list
//    //! \param statesList
//    //!
//    void setStatesList(const std::vector<Resource *> &statesList);

    //!
    //! \brief Getter for service
    //! \return ServerService instance
    //!
    ServerService *service() const;

    //!
    //! \brief Sets new ServerService instance
    //! \param service
    //!
    void setService(ServerService *service);

    //!
    //! \brief Sets new Engine instance
    //! \param engine
    //!
    void setEngine(AbstractEngine * engine);

    inline void clearResources() {if(m_statesList.count()) { qDeleteAll(m_statesList.begin(), m_statesList.end()); m_statesList.clear();}}
    inline void initializeResources();
    QString name() const;
    void setName(const QString &name);
    QVariantList getStateDataAsMap(const QString &clientName);

    Resource *getStateByName(const QString& name, bool *ok);
    Resource * getStateById(int stateId, bool *ok);


    void setSendCallback(const std::function<void (const QList<StateData> &)> &sendCallback);
    void sendChangesList(const QList<StateData> &);
    ServerService *getService() const;

signals:
    void pushState(const StateData &data);
    void stopAllSubsystems();
//    void propertyChanged(const QString & name, const QVariant & value);
public slots:
    void stop();
    void onResourceChanged(const StateData &);

private:
    ServerService * m_service = nullptr;
    SqliteProvider dbProvider;
    AbstractEngine * engine = nullptr;

    struct Call {
        AbstractSubsystem * subsystem = nullptr;
        QMetaMethod m;
    };

    QMap<QString, QList<Call>> m_methods;
    void addmethods(AbstractSubsystem * sys);


    std::function<void(const QList<StateData>&)> m_sendCallback = nullptr;

    //!
    //! \brief Global function that proceeds all subsystems and collects state changes
    //! \param data
    //! \return list of state data that sended to client
    //!
    //QList<StateData> globalProceed(const StateData & data);

};

#endif // STATION_H
