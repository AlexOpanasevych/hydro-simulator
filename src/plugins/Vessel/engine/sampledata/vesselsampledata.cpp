#include "vesselsampledata.h"

#include "QDataStream"

VesselSampleData::VesselSampleData()
{

}

VesselSampleData::VesselSampleData(std::vector<double> initPosition, double velocity, std::vector<double> acceleration, int uSecond) :
    PhysicalItemSampleData(initPosition, velocity, acceleration, uSecond){}

VesselSampleData::VesselSampleData(const VesselSampleData &data) : PhysicalItemSampleData(data.position, data.velocity, data.acceleration, data.uSecond)
{
}

std::string VesselSampleData::to_string()
{
    return "Vessel";
}

void VesselSampleData::deserialize(QDataStream *stream)
{
    PhysicalItemSampleData::deserialize(stream);
}

void VesselSampleData::serialize(QDataStream *stream) const
{
    PhysicalItemSampleData::serialize(stream);
}

void VesselSampleData::getAsVariant(QVariantMap &map)
{

}

QString VesselSampleData::getType()
{
    return this->m_type;
}






