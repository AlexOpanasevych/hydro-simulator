#ifndef SAMPLEDATA_H
#define SAMPLEDATA_H
#include <string>
#include "core_global.h"

#include <QDataStream>
#include <string>

struct CORE_LIBRARY_EXPORT SampleData
{
    long long uSecond;
    SampleData(){}
    SampleData(int uSecond) : uSecond(uSecond){}
    virtual std::string to_string() = 0;
    virtual ~SampleData(){}
    //inline static type = 12;
    virtual QString getType() = 0;
    virtual void serialize(QDataStream *) const;
    virtual void deserialize(QDataStream *);
    virtual void getAsVariant(QVariantMap & map);
};


#endif // SAMPLEDATA_H
