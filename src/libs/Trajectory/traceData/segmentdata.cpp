#include "segmentdata.h"

SegmentData::SegmentData(QObject *parent) : QObject(parent){}

int SegmentData::incBuffSize()
{
    setBufferSize(bufferSize() + 1);
    return bufferSize();
}

void SegmentData::setBuffSize(int size)
{
    setBufferSize(size);
}

int SegmentData::decBuffSize()
{
    setBufferSize(bufferSize() - 1);

    if (bufferSize() == 0)
    {
        setIsDead(true);
    }

    checkBufferCapacity();

    return bufferSize();
}

int SegmentData::pointsCount()
{
    return m_points.count();
}

void SegmentData::updateCoord(QPointF point)
{
    if (point != QPointF(5,15))
    m_points.push_front(point);
    checkBufferCapacity();
}

void SegmentData::press()
{
    emit pressed();
}

void SegmentData::checkBufferCapacity()
{
    int count = m_points.count();
    if (count > bufferSize())
    {
        m_points.remove(bufferSize(), count - bufferSize());
    }

    emit pointsChanged(m_points);
}

