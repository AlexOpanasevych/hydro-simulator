#include "physicalitemsampledata.h"
#include "sampledata.h"
#include "QDataStream"
#include <QVariantMap>
PhysicalItemSampleData::PhysicalItemSampleData()
{
    this->position.push_back(0);
    this->position.push_back(0);
    this->position.push_back(0);
    this->velocity = 0;
    this->acceleration.push_back(0);
    this->acceleration.push_back(0);
    this->acceleration.push_back(0);
}

PhysicalItemSampleData::PhysicalItemSampleData(std::vector<double> initPosition, double velocity, std::vector<double> acceleration, int uSecond) : SampleData(uSecond), position(initPosition), velocity(velocity), acceleration(acceleration)
{}

void PhysicalItemSampleData::setPosition(std::vector<double> newPosition)
{
#ifdef TESTEMPTY
    if(position.empty()) position.reserve(3);
#endif
    this->position[0] = newPosition[0];
    this->position[1] = newPosition[1];
    this->position[2] = newPosition[2];
}
void PhysicalItemSampleData::setVelocity(double newVelocity){

#ifdef TESTEMPTY
    if(velocity.empty()) velocity.reserve(3);
#endif
    this->velocity = newVelocity;

}

void PhysicalItemSampleData::setAcceleration(std::vector<double> newAcceleration)
{
#ifdef TESTEMPTY
    if(acceleration.empty()) acceleration.reserve(3);
#endif
    this->acceleration[0] = newAcceleration[0];
    this->acceleration[1] = newAcceleration[1];
    this->acceleration[2] = newAcceleration[2];
}

PhysicalItemSampleData::~PhysicalItemSampleData()
{

}

void PhysicalItemSampleData::deserialize(QDataStream *stream)
{
    *stream >> uSecond;
    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
    *stream >> position[0];
    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
    *stream >> position[1];
    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
    *stream >> position[2];
    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
    *stream >> acceleration[0];
    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
    *stream >> acceleration[1];
    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
    *stream >> acceleration[2];
    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
    *stream >> velocity;
    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
    *stream >> direction[0];
    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
    *stream >> direction[1];
    stream->setFloatingPointPrecision(QDataStream::DoublePrecision);
    *stream >> direction[2];
}

void PhysicalItemSampleData::serialize(QDataStream *stream) const
{
    *stream << uSecond;
    *stream << position[0];
    *stream << position[1];
    *stream << position[2];
    *stream << acceleration[0];
    *stream << acceleration[1];
    *stream << acceleration[2];
    *stream << velocity;
    *stream << direction[0];
    *stream << direction[1];
    *stream << direction[2];

}

void PhysicalItemSampleData::getAsVariant(QVariantMap &map)
{
    SampleData::getAsVariant(map);
    map["dead"] = this->destroyed;
//    map[""]
}

std::vector<double> operator *(const std::vector<double> & left, double right)
{
    std::vector<double> result;
    result.push_back(left[0] * right);
    result.push_back(left[1] * right);
    result.push_back(left[2] * right);
    return result;
}

void operator +=(std::vector<double> & left, std::vector<double> & right)
{
    left[0] += right[0];
    left[1] += right[1];
    left[2] += right[2];
//    qDebug() << "Left" <<  left << "right" << right << Qt::endl;
//    qDebug() << "---" << Qt::endl;
}

std::vector<double> operator *(std::vector<double> & left, std::vector<double> & right){
    return {left[1] * right[2] - left[2]*right[1], left[2] * right[0] - left[0] * right[2], left[0] * right[1] - left[1] * right[0]};
}

void operator *=(std::vector<double> & left, std::vector<double> & right){
    left[0] = left[1] * right[2] - left[2] * right[1];
    left[1] = left[2] * right[0] - left[0] * right[2];
    left[2] = left[0] * right[1] - left[1] * right[0];
}


std::vector<double> operator +(const std::vector<double> &left, const std::vector<double> &right)
{
    return {left[0] + right[0], left[1] + right[1], left[2] + right[2]};
}
