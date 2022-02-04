#ifndef EPO160PAINTER_H
#define EPO160PAINTER_H

#include "epo1painter.h"
#include <streamconnector.h>

class Epo160Painter : public EPO1Painter
{
//    Q_OBJECT
public:
    Epo160Painter(MGKAbstractMonitor * parent);


    // MonitorPainter interface
public:
//    void paint(QPainter *painter) override;
};

#endif // EPO160PAINTER_H
