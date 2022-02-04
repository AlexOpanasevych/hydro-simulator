#include "changevelocitymsg.h"
#include <QDataStream>

void ChangeVelocityMsg::serialize(QDataStream * s) const
{
    *s << this->vel_x << this->vel_y << this->vel_z;
}

void ChangeVelocityMsg::deserialize(QDataStream * s)
{
    *s >> this->vel_x >> this->vel_y >> this->vel_z;
}

void ChangeVelocityMsgAns::serialize(QDataStream * s) const
{
    *s << this->success;
}

void ChangeVelocityMsgAns::deserialize(QDataStream * s)
{
    *s >> this->success;
}
