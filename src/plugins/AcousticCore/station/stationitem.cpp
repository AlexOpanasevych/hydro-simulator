#include "stationitem.h"
#include <math.h>
#include <QPainterPath>
#include <QPainter>
#include <QRectF>

StationItem::StationItem(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    setWidth(1000);
    setHeight(1000);
    connect(this, &StationItem::shiftAngleChanged, this, [this](){ this->update(); });
    connect(this, &StationItem::angleChanged, this, [this](){ this->update(); });
    connect(this, &StationItem::xnCountChanged, this, [this](){ this->update(); });
}

void StationItem::paint(QPainter *painter)
{
    painter->translate(width() / 2, height() / 2);
    QPainterPath xnPath;
    //creating one XN
    const int n = 10;
    double massR[n+1];
    /* W -> coefficient for cos(5 * w)=0.8 */
    //auto w = 7.37398;
    auto w = acos(0.8) / (m_angle * M_PI / 180);
    double step = M_PI / (w * 2 * n);

    QVector<QPointF> points;
    for(int i = 0; i <= n; i++) {
        auto phi = i * step;
        //phi *= w;
        massR[i] = 400 * cos(w * phi);
        points.append({ massR[i] * sin(phi), -massR[i] * cos(phi) });
    }

    //painting first half
    bool frst = true;
    for(auto p: points) {
        if (frst) {
            xnPath.moveTo(p);
            frst = false;
        } else {
            xnPath.lineTo(p);
        }
    }
    //painting second half
    for(int i = points.size() - 1; i >= 0; i--)
    {
        xnPath.lineTo(-points.at(i).x(), points.at(i).y());
    }

    painter->setPen(QPen(Qt::transparent));
    painter->setBrush(QBrush(QColor(255,  0, 0, 15)));
    qreal mainShift = (m_xnCount - 1) * (m_shiftAngle / 2);
    xnPath = QTransform().rotate(90 - mainShift).map(xnPath);

    for (int i = 0; i < m_xnCount; i++)
    {
        QPainterPath RotatedPath;
        QTransform shiftAngle;
        shiftAngle.rotate(m_shiftAngle * i);
        RotatedPath = shiftAngle.map(xnPath);
        painter->drawPath(RotatedPath);
    }
}

int StationItem::xnCount() const
{
    return m_xnCount;
}

double StationItem::angle() const
{
    return m_angle;
}

double StationItem::shiftAngle() const
{
    return m_shiftAngle;
}

void StationItem::setXnCount(int xnCount)
{
    if (m_xnCount == xnCount)
        return;

    m_xnCount = xnCount;
    emit xnCountChanged(m_xnCount);
}

void StationItem::setAngle(double angle)
{
    if (qFuzzyCompare(m_angle, angle))
        return;

    m_angle = angle;
    emit angleChanged(m_angle);
}

void StationItem::setShiftAngle(double shiftAngle)
{
    if (qFuzzyCompare(m_shiftAngle, shiftAngle))
        return;

    m_shiftAngle = shiftAngle;
    emit shiftAngleChanged(m_shiftAngle);
}
