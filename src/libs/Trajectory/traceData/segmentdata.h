#ifndef SEGMENTDATA_H
#define SEGMENTDATA_H

#include <QObject>
#include <QPointF>
#include <libs/klibcorelite/kmacro.h>
#include <trajectory_global.h>

class Trace;
class TRAJECTORY_EXPORT SegmentData : public QObject
{
    Q_OBJECT
    K_READONLY_PROPERTY(int, bufferSize, bufferSize, setBufferSize, bufferSizeChanged, 100000)
    K_READONLY_PROPERTY(bool, isDeaad, isDead, setIsDead, isDeadChanged, false)
    K_READONLY_PROPERTY(QVector<QPointF>, points, points, setPoints, pointsChanged, QVector<QPointF>())
    K_READONLY_PROPERTY(bool, checked, checked, setChecked, checkedChanged, false)
    K_READONLY_PROPERTY(QPointF, emittingPoint, emittingPoint, setEmittingPoint, emittingPointChanged, QPointF(0,0))
    friend Trace;
public:
    explicit SegmentData(QObject *parent = nullptr);

public slots:
    int incBuffSize();
    void setBuffSize(int size);
    int decBuffSize();
    int pointsCount();
    void updateCoord(QPointF point);
    void press();
signals:
    void pointsChanged(QPointF m_points);
    void pressed();

private:
    void checkBufferCapacity();
    //QVector<QPointF> m_points;

};

#endif // SEGMENTDATA_H
