#ifndef TUCPAINTER_H
#define TUCPAINTER_H

//#include "src/lib/ktypes/kdouble.h"
//include "src/lib/ktools/kpropertycontext.h"
#include "monitorpainter.h"

#include <items/mgkabstractmonitor.h>
#include <QElapsedTimer>
#include <kmacro.h>

class TucPainter : public MonitorPainter {
    double currentTargetBearing = 10;
    double currentTargetPressure = 0.5;
    double bearing = 30;
    int linesCount = 15;
    int yDeviation = 60;
public:
    TucPainter(MGKAbstractMonitor * parent);

    // MonitorPainter interface
public:
    void paint(QPainter *painter) override;
//    int maxTime = 5000;
//    double currentTime = 0;
//    QElapsedTimer timer;
};

#endif // TUCPAINTER_H
