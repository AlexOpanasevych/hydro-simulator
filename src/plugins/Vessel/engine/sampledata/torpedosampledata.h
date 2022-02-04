#ifndef TORPEDOSAMPLEDATA_H
#define TORPEDOSAMPLEDATA_H

#include <QDataStream>
#include <libs/core/physicalitemsampledata.h>
#include <torpedo_global.h>

class SampleData;

struct TORPEDO_EXPORT TorpedoSampleData : public PhysicalItemSampleData
{
    TorpedoSampleData(std::vector<double>initPosition);
    TorpedoSampleData(double velocity, std::vector<double>acceleration);
    TorpedoSampleData(std::vector<double>initPosition, double velocity, std::vector<double>acceleration, int uSecond);
    TorpedoSampleData(const TorpedoSampleData &data);
    std::string to_string() override;
    QString getType() override;
    TorpedoSampleData();
    void deserialize(QDataStream * stream) override;
    void serialize(QDataStream * stream) const override;
private:
    inline static QString m_type = "Torpedo";
};





#endif // TORPEDOSAMPLEDATA_H
