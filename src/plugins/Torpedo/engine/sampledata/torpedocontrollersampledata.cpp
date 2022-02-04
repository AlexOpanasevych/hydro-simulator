#include "torpedocontrollersampledata.h"

std::string TorpedoControllerSampleData::to_string()
{
    return "tcsdata";
}

QString TorpedoControllerSampleData::getType()
{
    return m_type;
}

TorpedoControllerSampleData::TorpedoControllerSampleData() : piId(0), alpha(0), theta(0)
{

}

void TorpedoControllerSampleData::getAsVariant(QVariantMap &map)
{

}
