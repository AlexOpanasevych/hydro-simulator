#ifndef ABSTRACTPICONTROLLER_H
#define ABSTRACTPICONTROLLER_H

#include "basemsg.h"
#include "messagedispatcher.h"
#include "core_global.h"
#include "abstractengine.h"

class AbstractEngine;

//!
//! \brief Abstract base class for all physical item controllers
//!
class CORE_LIBRARY_EXPORT AbstractPIController
{
public:
    AbstractPIController();
    virtual ~AbstractPIController();

    //!
    //! \brief Get physical item id from controller
    //! \return physical item id for this controller
    //!
    int getPiId() const { return piId; }

    //!
    //! \brief Sets the physical item id
    //! \param value
    //!
    void setPiId(int value);
    //!
    //! \brief Sets the engine for this controller
    //! \param e
    //!
    void setEngine(AbstractEngine *e);

    //!
    //! \brief Proceeds current tick in engine
    //!
    virtual void proceed(long long, double dt) = 0;


    virtual void proceedAfter(long long, double dt) = 0;

    //!
    //! \brief Get info about current tick in engine
    //! \return
    //!
    virtual QVariantMap getInfo() = 0;

    //!
    //! \brief Executes MessageDispatcher method \a proceedMsg
    //! \param h
    //! \param s
    //! \return QByteArray which contains answer
    //!
    QByteArray proceedMsg(Header *h, QDataStream &s);

    //!
    //! \brief Initializes controller with given msg, and engine
    //! \param msg
    //! \param e
    //!
    virtual void init(BaseMsg * msg, AbstractEngine * e) = 0;

    virtual QString name() = 0;
protected:
    //!
    //! \brief e
    //!
    AbstractEngine *e;
    //!
    //! \brief PhysicalItem ID
    //!
    int piId;
//    int prvTime;

    //!
    //! \brief mDisp
    //!
    MessageDispatcher * mDisp;
};

#endif // ABSTRACTCONTROLLER_H
