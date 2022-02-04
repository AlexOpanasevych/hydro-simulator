#include "changeanglemsg.h"
#include <QDataStream>

void ChangeAngleMsg::serialize(QDataStream *) const
{

}

void ChangeAngleMsg::deserialize(QDataStream *)
{

}

void ChangeAngleMsgAns::serialize(QDataStream * s) const
{
    *s << this->changed;
}

void ChangeAngleMsgAns::deserialize(QDataStream * s)
{
    *s >> this->changed;
}
