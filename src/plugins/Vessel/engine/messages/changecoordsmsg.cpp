#include "changecoordsmsg.h"
#include <QDataStream>

void ChangeCoordsMsg::serialize(QDataStream *s) const
{
    *s << this->x << this->y << this->z;
}

void ChangeCoordsMsg::deserialize(QDataStream * s)
{
    *s >> this->x >> this->y >> this->z;
}

void ChangeCoordsMsgAns::serialize(QDataStream * s) const
{;
    *s << this->success;
}

void ChangeCoordsMsgAns::deserialize(QDataStream * s)
{
    *s >> this->success;
}
