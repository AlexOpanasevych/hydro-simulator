#ifndef BRONZESWEEPPAINTER_H
#define BRONZESWEEPPAINTER_H

#include "sweeppainter.h"

class BronzeSweepPainter : public SweepPainter
{
    Q_OBJECT
public:
    BronzeSweepPainter(MGKAbstractMonitor * parent = nullptr);

    // MonitorPainter interface
public:
    void paint(QPainter *painter);
};

#endif // BRONZESWEEPPAINTER_H
