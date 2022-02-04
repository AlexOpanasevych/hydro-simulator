#ifndef SIMULATION_H
#define SIMULATION_H

#include "src/qcustomplot/qcustomplot.h"
#include "raycalculator.h"
#include <QAudioFormat>
#include <QAudioOutput>
#include <QList>
#include <src/audio/rawsound.h>
#include <src/lib/audio/ksounddevice.h>
#include <src/lib/audio/ksoundmix.h>

#define MAGIC_COEFFICIENT 10



class Simulation : public QObject
{
    Q_OBJECT
    typedef QVector<QVector<QPair<int, QVector<QPair<QVector<double>, QVector<double>>>>>> RayTracerBuffer;

    //hidrology type
    AUTO_PROPERTY(int, season, season, setSeason, seasonChanged, 0)
    AUTO_FLOAT_PROPERTY(double, depth, depth, setDepth, depthChanged, 0)
    AUTO_PROPERTY(int, linesCount, linesCount, setLinesCount, linesCountChanged, 5)
    //position (distance, depth)
    AUTO_PROPERTY(QPointF, submarinePos, submarinePos, setSubmarinePos, submarinePosChanged, QPointF(0, 0))
    AUTO_PROPERTY(bool, reflected, reflected, setReflected, reflectedChanged, false)
    // false -> hydroloy off
    AUTO_PROPERTY(bool, hydrologyState, hydrologyState, setHydrologyState, hydrologyStateChanged, true)

    static QString bufferPath();
public:
    explicit Simulation(QObject *parent = nullptr);

    void start();
    void calculate(bool calc = true);
    void calculateWave();
    void calculateSpeed();
    void calculateBackWave();

    bool isReflected(double distance, double depth);
    bool isRecieved(double distance, double depth);

    //getters && setters
    QCustomPlot *getSoundSpeedPlot() const;
    void setSoundSpeedPlot(QCustomPlot *value);

    QCustomPlot *getRayPlot() const;
    void setRayPlot(QCustomPlot *value);

    QPair<QVector<double>, QVector<double>> *getSoundSpeedCurve();
    QPair<QVector<double>, QVector<double>> *getRayCurve();
    QList<QPair<QVector<double>, QVector<double>>> *getGraphPointsList();
    QList<QPair<QVector<double>, QVector<double>>> *getBackGraphPointsList();
    QPair<QVector<double>, QVector<double>> *getBackRayCurve();

private:
    QPair<QVector<double>, QVector<double>>         rayCurve;
    QPair<QVector<double>, QVector<double>>         backRayCurve;
    QPair<QVector<double>, QVector<double>>         soundSpeedCurve;
    QList<QPair<QVector<double>, QVector<double>>>  graphPointsList;
    QList<QPair<QVector<double>, QVector<double>>>  backGraphPointsList;

    RayTracerBuffer buffer;

    void createBuffer();
    void loadFromBuffer();

    QVector<QPair<QVector<double>, QVector<double>>> findCoordsByDepth(int depth);

    QCustomPlot *soundSpeedPlot = nullptr;
    QCustomPlot *rayPlot = nullptr;
    double lastR = 30000;
    double thetaMax = 5;
private:
    //tools
    double FindCoordinate(double , QVector<double>);
    double interpolate(QVector<double> xData, QVector<double> yData, double x, bool exterpolate = false);
    QVector<double> constrainVector(double start, double end, int size);

signals:
    void repaint();

private slots:
    void repaintClicked();
};

#endif // SIMULATION_H
