#include "receiver.h"
QTimer *Receiver::timer() const
{
    return m_timer;
}

Receiver::Receiver(QObject *parent) : AbstractSubsystem(parent)
{



}

int Receiver::order()
{

}
