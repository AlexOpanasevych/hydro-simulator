#ifndef ABSTRACTSUBSYSTEM_H
#define ABSTRACTSUBSYSTEM_H

#include "state.h"
#include "../serverinterface_global.h"
#include <QRunnable>
#include <vector>
#include <utility/kobservablelist.h>

class AbstractEngine;
class Station;

class SERVERINTERFACE_LIBRARY_EXPORT AbstractSubsystem : public QObject
{
    Q_OBJECT
    K_AUTO_PROPERTY(Station*, station, station, setStation, stationChanged, nullptr)
public:
    explicit AbstractSubsystem(QObject * parent = nullptr);

    //!
    //! \brief Sets engine
    //! \param engine
    //!
    void setEngine(AbstractEngine *engine);

    //!
    //! \brief order
    //! \return index in order (?)
    //!
    virtual int order() = 0;
public slots:

    //!
    //! \brief Proceeds states changes from client
    //! \param stateList
    //! \param data
    //! \return list of internal states changes that  must be sended to client
    //!
    virtual void proceed(KObservableList<Resource> & stateList, const StateData & data) = 0;

    virtual void init() = 0;

protected:
    AbstractEngine * m_engine = nullptr;
};

#endif // ABSTRACTSUBSYSTEM_H
