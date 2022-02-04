#include "abstractpicontroller.h"

AbstractPIController::AbstractPIController()
{
    this->mDisp = new MessageDispatcher;
//    prvTime = 0;
}

AbstractPIController::~AbstractPIController()
{
    delete this->mDisp;
}

void AbstractPIController::setPiId(int value)
{
    piId = value;
}

void AbstractPIController::setEngine(AbstractEngine *value)
{
    e = value;
}

QByteArray AbstractPIController::proceedMsg(Header* h, QDataStream &s)
{
    return this->mDisp->proceedMsg(h, s);
}
