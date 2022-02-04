#ifndef WAVEDATA_H
#define WAVEDATA_H

#include <QPointF>

struct WaveData{
public:
    QPointF pos;
    double radius;
    double startAngle;
    double arcRadius;
    double lineWidth;
    int creationTime;
    bool dead = false;
};

#endif // WAVEDATA_H
