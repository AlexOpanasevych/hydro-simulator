#include "settorpedoparams.h"

SetTorpedoParams::SetTorpedoParams()
{

}

void SetTorpedoParams::serialize(QDataStream *s) const
{
    *s << firstRA << secondRA << torpedoDivergency << rotateDistance;
}

void SetTorpedoParams::deserialize(QDataStream *s)
{
    *s >> firstRA >> secondRA >> torpedoDivergency >> rotateDistance;

}

SetTorpedoParamsAns::SetTorpedoParamsAns()
{

}

void SetTorpedoParamsAns::serialize(QDataStream *s) const
{
    *s << success;
}

void SetTorpedoParamsAns::deserialize(QDataStream *s)
{
    *s >> success;
}
