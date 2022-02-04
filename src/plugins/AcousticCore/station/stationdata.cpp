#include "stationdata.h"

StationData::StationData(QObject *parent) : QObject(parent)
{

}

double StationData::stationRotation() const
{
    return m_stationRotation;
}

double StationData::x() const
{
    return m_x;
}

double StationData::y() const
{
    return m_y;
}

double StationData::rotation() const
{
    return m_rotation;
}

void StationData::move(QPointF pos)
{
    setX(pos.x());
    setY(pos.y());
}

void StationData::setStationRotation(double stationRotation)
{
    if (qFuzzyCompare(m_stationRotation, stationRotation))
        return;

    m_stationRotation = stationRotation;
    emit stationRotationChanged(m_stationRotation);
}

void StationData::setY(double y)
{
    if (qFuzzyCompare(m_y, y))
        return;

    m_y = y;
    emit yChanged(m_y);
}

void StationData::setX(double x)
{
    if (qFuzzyCompare(m_x, x))
        return;

    m_x = x;
    emit xChanged(m_x);
}

PhysicalItem *StationData::sceneItem() const
{
    return m_sceneItem;
}

void StationData::setSceneItem(PhysicalItem *sceneItem)
{
    if (m_sceneItem == sceneItem)
        return;

    m_sceneItem = sceneItem;
    emit sceneItemChanged(m_sceneItem);
}

void StationData::setRotation(double angle)
{
    if (qFuzzyCompare(m_rotation, angle))
        return;

    m_rotation = angle;
    emit rotationChanged(m_rotation);
}

StationData::Mode StationData::mode() const
{
    return m_mode;
}

void StationData::setMode(StationData::Mode mode)
{
    if (m_mode == mode)
        return;
    switch (mode) {
    case StationData::PLATINA:{
        setStationRotation(0);
        setXnCount(12);
        setColor(QColor("red"));
        break;
    }
    case StationData::BRONZE:{
        setStationRotation(180);
        setXnCount(4);
        setColor(QColor("brown"));
        break;
    }
    }
    m_mode = mode;
    emit modeChanged(m_mode);
}

int StationData::xnCount() const
{
    return m_xnCount;
}

void StationData::setXnCount(int newXnCount)
{
    if (m_xnCount == newXnCount)
        return;
    m_xnCount = newXnCount;
    emit xnCountChanged();
}

const QColor &StationData::color() const
{
    return m_color;
}

void StationData::setColor(const QColor &newColor)
{
    if (m_color == newColor)
        return;
    m_color = newColor;
    emit colorChanged();
}
