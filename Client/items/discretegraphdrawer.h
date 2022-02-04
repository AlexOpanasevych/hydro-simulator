#ifndef DISCRETEGRAPHDRAWER_H
#define DISCRETEGRAPHDRAWER_H

#include <QPainter>

#include <utility/kintegrator.h>
#include <utility/krandomstep.h>
#include <qnanopainter.h>

#include <items/painters/monitorpainter.h>

class DiscreteGraphDrawer {
    QVector<double> m_values;
    QPoint m_position;
    int width;
    int top;
    int bottom;
    double max;
    double min;
    int sight = 1;
    KRandomStep generator;
    KIntegrator integrator;
public:
    DiscreteGraphDrawer(int width, int top, int bottom, double min, double max);

    void paint(MonitorPainter *mp);
    QVector<double> values() const;
    void setValues(const QVector<double> &values);
    void setSight(int value);
    QPoint position() const;
    void setPosition(const QPoint &position);
};

#endif // DISCRETEGRAPHDRAWER_H
