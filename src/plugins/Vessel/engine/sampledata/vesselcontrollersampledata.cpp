#include "vesselcontrollersampledata.h"

VesselControllerSampleData::VesselControllerSampleData()
{

}

QString VesselControllerSampleData::getType()
{
    return this->m_type;
}

void VesselControllerSampleData::deserialize(QDataStream * s)
{

}

std::string VesselControllerSampleData::to_string()
{
    return "VesselController";
}

void VesselControllerSampleData::serialize(QDataStream * s) const
{

}

void VesselControllerSampleData::getAsVariant(QVariantMap &map)
{

}
