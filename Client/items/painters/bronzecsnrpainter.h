#ifndef BRONZECSNRPAINTER_H
#define BRONZECSNRPAINTER_H

#include "monitorpainter.h"

#include <utility/kdiscretizer.h>
#include <utility/kintegrator.h>
#include <utility/propamplifier.h>

#include <qnanopainter.h>
#include <streamconnector.h>

struct Gainer {
    std::function<double(QVariant)> converter = nullptr;
    std::function<double(double)> gain = nullptr;
};

class BronzeCSNRPainter : public MonitorPainter
{
    Q_OBJECT
    QVector<double> input;

    KIntegrator integrator;

    KDiscretizer discretizer;

    double bronzeSight = 0.5;
    double sight = 0.5;
//    int bronzeMode = ;
//    double bronzeAutoSight;
    double bronzeCurrentPressure;

    int lastI = 0;


    double gainTo = 0;
    int gainLevelMultiplier = 4;
    double limit = std::numeric_limits<double>::max();

    Gainer TOatinuator;
//    PropAmplifier amplifier;
//    PropAmplifier RU1Atinuator;
//    PropAmplifier RU2Atinuator;
//    PropAmplifier TOAtinuator;

public:
    BronzeCSNRPainter(MGKAbstractMonitor *parent);
    void paint(QPainter* painter) override;
    listener m_handler = nullptr;


    double gain(double signal);

public slots:
    // gain converter
    void onGainTOChanged();

};

#endif // BRONZECSNRPAINTER_H
