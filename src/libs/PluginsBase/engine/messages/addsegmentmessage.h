#ifndef ADDSEGMENTMESSAGE_H
#define ADDSEGMENTMESSAGE_H

#include <basemsg.h>



struct SetStageMessage : public BaseMsg
{
public:
    QString stage;
    void serialize(QDataStream *s) const override;
    void deserialize(QDataStream *s) override;
};

#endif // ADDSEGMENTMESSAGE_H
