#include "createpimsg.h"
#include <QDataStream>

void CreatePIMsg::serialize(QDataStream * s) const
{
    this->getType();
    *s << this->position[0] << this->position[1] << this->position[2]
            << this->velocity << this->angle << this->typeOfObject;
}

void CreatePIMsg::deserialize(QDataStream *s)
{
    *s >> this->position[0]>> this->position[1] >> this->position[2] >> this->velocity >> this->angle >> this->typeOfObject;
}

void CreatePIMsgAns::serialize(QDataStream * data) const
{
    *data << createdId << this->position[0] << this->position[1] << this->position[2]
            << this->velocity << this->angle << this->typeOfObject;
}

void CreatePIMsgAns::deserialize(QDataStream *s)
{
    *s >> createdId >> this->position[0] >> this->position[1] >> this->position[2] >> this->velocity >> this->angle >> this->typeOfObject;
}
