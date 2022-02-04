#include "savedata.h"

#include <QBuffer>
#include <QPixmap>
#include <QVariant>

QVariant SaveData::serialize()
{
    return QVariantMap{
        { "saveData"    , saveData()    },
        { "saveName"    , saveName()    },
        { "description" , description() },
        { "date"        , date()        },
        { "idx"         , idx()         }
    };
}

void SaveData::deserialize(QVariant data)
{
    setSaveData(data.toMap().value("saveData", "error"));
    setSaveName(data.toMap().value("saveName", "error").toString());
    setDescription(data.toMap().value("description", "error").toString());
    setDate(data.toMap().value("date", "error").toString());
    setIdx(data.toMap().value("idx", -1).toInt());
}
