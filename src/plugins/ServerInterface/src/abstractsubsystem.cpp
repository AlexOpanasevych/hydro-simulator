#include "abstractsubsystem.h"

#include <abstractengine.h>

AbstractSubsystem::AbstractSubsystem(QObject *parent) : QObject(parent)
{

}

void AbstractSubsystem::setEngine(AbstractEngine *engine)
{
    m_engine = engine;
}
