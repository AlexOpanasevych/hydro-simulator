#ifndef SIMULATIONGRAPHIC_H
#define SIMULATIONGRAPHIC_H

#include "raycalculator.h"
#include "simulation.h"

#include <QObject>
#include <src/qcustomplot/qcustomplot.h>

class SimulationGraphic : public QObject
{
    Q_OBJECT
public:
    SimulationGraphic(QCustomPlot *sSPlot, QCustomPlot *rPlot, Simulation *simul, QObject *parent = nullptr);
    void paint();
    void paintSpeed();
    void paintSubmarine();
    QTimer *timer;
    Simulation *simulation;
    QPair<QVector<double>, QVector<double>> *soundSpeedCurve;
    QPair<QVector<double>, QVector<double>> *rayCurve;
    QPair<QVector<double>, QVector<double>> *backRayCurve;
    QCustomPlot *soundSpeedPlot;
    QCustomPlot *rayPlot;
    QQmlContext *controlPanel;
    QList<QPair<QVector<double>, QVector<double>>> *graphList;
    QList<QPair<QVector<double>, QVector<double>>> *backGraphList;
    bool rescale = true;
    bool posChanged = true;

public slots:
    void repaint();
};

#endif // SIMULATIONGRAPHIC_H
