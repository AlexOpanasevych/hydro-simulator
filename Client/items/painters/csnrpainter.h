#ifndef CSNRPAINTER_H
#define CSNRPAINTER_H

#include "monitorpainter.h"

#include <QElapsedTimer>
#include <QQueue>
#include <math.h>

#include <utility/kdiscretizer.h>
#include <utility/kintegrator.h>
#include <streamconnector.h>

struct DataPoint {
    double time;
    double value;
};

class CSNRPainter : public MonitorPainter
{
    double sight = 1.0;
    double sightBrightness = 1.0;
    double currentTargetPressure = 1.0;

//    QVector<double> input;
    QVector<double> input;
    double currentAngle = 0;
    QElapsedTimer elapsed;

    int lastI = 0;
//    int points = ceil(200 * 360 / 5000.);
//    std::array<double, 360> currentPressures;

    KIntegrator integrator;
    KDiscretizer discretizer;

    listener m_listener = nullptr;
//    void pushData(const QVector<double> &p);
    static constexpr auto maxTime = 5000;

public:
    CSNRPainter(MGKAbstractMonitor * parent);

    void paint(QPainter * painter) override;

    void drawSight(uint32_t * pixels, double sight, double minRadius, double maxRadius);
};

#endif // CSNRPAINTER_H
