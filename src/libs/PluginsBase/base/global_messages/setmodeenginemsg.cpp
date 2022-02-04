#include "setmodeenginemsg.h"

void SetModeEngineMsgAns::serialize(QDataStream *s) const
{
    *s << this->modeChangedSuccess << this->mode;
}

void SetModeEngineMsgAns::deserialize(QDataStream *s)
{
    *s >> this->modeChangedSuccess;
    int mode;
    *s >> mode;
    this->mode = static_cast<EngineMode>(mode);
}

void SetModeEngineMsg::serialize(QDataStream *s) const
{
    *s << this->mode;
}

void SetModeEngineMsg::deserialize(QDataStream *s)
{
    int mode;
    *s >> mode;
    this->mode = static_cast<EngineMode>(mode);
}


