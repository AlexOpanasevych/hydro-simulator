#include "trace.h"
#include <cdebug.h>

Trace::Trace(QObject *parent) : QObject(parent)
{
    m_segments = m_segmentsList.model();
    m_segments->setParent(this);
}

void Trace::deserialize(QVariant)
{

}

void Trace::updateCoords(QPointF point, Trace::TraceMode mode)
{
    switch (mode) {
        case Trace::START_SEGMENT: {
            if (m_segmentsList.count() > 0)
            {
                auto segment = m_segmentsList.first();
                segment->setBuffSize(segment->pointsCount());
            }
            auto segment = new SegmentData();
            connect(segment, &SegmentData::pressed, this, [this, segment](){
                bool ch = segment->checked();
                disselectAllItems();
                segment->setChecked(!ch);
            });
            segment->setEmittingPoint(point);
            segment->updateCoord(point);
            m_segmentsList.push_front(segment);
            qDebug() << "Created segments" << m_segmentsList.count();
            break;
        }
        case Trace::EXPAND_SEGMENT: {
            if (m_segmentsList.count() > 0)
            {
                auto segment = m_segmentsList.first();
                segment->updateCoord(point);
            }
            else
            {
                auto segment = new SegmentData();
                segment->updateCoord(point);
                m_segmentsList.push_front(segment);
            }
            break;
        }
        case Trace::END_SEGMENT: {
            if (m_segmentsList.count() > 0)
            {
                auto segment = m_segmentsList.first();
                segment->updateCoord(point);
                segment->setBuffSize(segment->pointsCount());
            }
            break;
        }
        case Trace::DEFAULT: {
            break;
        }
    }

    if (point != QPointF(5,15))
    {
        m_points.push_front(point);
    }

    //уменьшаем "хвост"
    if (m_segmentsList.count() > 0 && m_points.count() > bufferSize())
    {
        m_points.remove(bufferSize(), m_points.count() - bufferSize());
        auto segment = m_segmentsList.last();
        segment->decBuffSize();
    }

    for (int i = m_segmentsList.count() - 1; i >= 0; i--)
    {
        auto segment = m_segmentsList.get(i);
        if (segment->isDead()) {
            m_segmentsList.remove(i);
            disconnect(segment, nullptr, this, nullptr);
            segment->setChecked(false);
            delete segment;
            qDebug(v) << "deleted";
        }
    }

    emit coordsUpdated(point);

    //addToBuffer(point);
}

void Trace::setCoords(QVector<QPointF> point, Trace::TraceMode mode)
{
    Q_UNUSED(point)
    Q_UNUSED(mode)

    //m_points.clear();
}

void Trace::updateCoords(double width, double height, Trace::TraceMode mode)
{
    updateCoords(QPointF(width, height), mode);
}

void Trace::disselectAllItems()
{
    for (auto segment : m_segmentsList)
    {
        segment->setChecked(false);
    }
}

void Trace::addToBuffer(QPointF point)
{
    if (point != QPointF(5,15))
    m_points.push_front(point);
    int count = m_points.count();
    if (count > bufferSize())
    {
        m_points.remove(bufferSize(), count - bufferSize());
    }

}
