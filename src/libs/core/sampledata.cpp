#include "sampledata.h"
#include <QVariantMap>

void SampleData::deserialize(QDataStream *){}

void SampleData::getAsVariant(QVariantMap &map)
{
    map["uSecond"] = uSecond;
}

void SampleData::serialize(QDataStream *) const{}
