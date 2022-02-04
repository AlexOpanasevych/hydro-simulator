#include "addsegmentmessage.h"


void SetStageMessage::serialize(QDataStream *s) const
{
    *s << stage.length() << stage;
}

void SetStageMessage::deserialize(QDataStream *s)
{
    int stLength = 0;
    QString stage;
    *s >> stLength;
    //s->readRawData()
}
