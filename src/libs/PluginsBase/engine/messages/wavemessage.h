#ifndef WAVEMESSAGE_H
#define WAVEMESSAGE_H

#include <basemsg.h>

struct WaveMessage : public BaseMsg
{
public:
    WaveMessage();
};

struct WaveMessageAns : public BaseMsg
{
public:
    WaveMessageAns();
};

#endif // WAVEMESSAGE_H
