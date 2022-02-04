#include "torpedosampledata.h"
#include <QVariantMap>

TorpedoSampleData::TorpedoSampleData(std::vector<double> initPosition)
{
    Q_UNUSED(initPosition)
}

TorpedoSampleData::TorpedoSampleData(double velocity, std::vector<double> acceleration)
{
    Q_UNUSED(velocity)
    Q_UNUSED(acceleration)
}

TorpedoSampleData::TorpedoSampleData(std::vector<double> initPosition, double velocity, std::vector<double> acceleration, int uSecond) :

    PhysicalItemSampleData(initPosition, velocity, acceleration, uSecond){this->uSecond = uSecond;}

TorpedoSampleData::TorpedoSampleData(const TorpedoSampleData &data) : PhysicalItemSampleData(data.position, data.velocity, data.acceleration, data.uSecond)
{
}

std::string TorpedoSampleData::to_string()
{
    return "Torpedo";
}

QString TorpedoSampleData::getType()
{
    return this->m_type;
}

TorpedoSampleData::TorpedoSampleData() : PhysicalItemSampleData({0, 0, 0}, 0, {0, 0, 0}, 0){}

void TorpedoSampleData::deserialize(QDataStream *stream)
{
//    *stream >> uSecond;
//    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
//    *stream >> position[0];
//    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
//    *stream >> position[1];
//    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
//    *stream >> position[2];
//    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
//    *stream >> acceleration[0];
//    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
//    *stream >> acceleration[1];
//    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
//    *stream >> acceleration[2];
//    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
//    *stream >> velocity;

    PhysicalItemSampleData::deserialize(stream);
}

void TorpedoSampleData::serialize(QDataStream *stream) const
{
//    *stream << uSecond;
//    *stream << position[0];
//    *stream << position[1];
//    *stream << position[2];
//    *stream << acceleration[0];
//    *stream << acceleration[1];
//    *stream << acceleration[2];
//    *stream << velocity;
    PhysicalItemSampleData::serialize(stream);
}

void TorpedoSampleData::getAsVariant(QVariantMap &map)
{
    PhysicalItemSampleData::getAsVariant(map);
    map["wIndex"] = wIndex;
    map["angleVelocity"] = angleVelocity;
    map["distanceToGoal"] = distanceToGoal;
    map["currentDistance"] = currentDistance;
    map["beta"] = beta;
    map["onLife"] = onLife;
}
