#include "bronzecsnrpainter.h"

#include <math/kmath.h>
#include <math.h>
#include <items/monitors/mgkbronzerectmonitor.h>
//#include <streamconnector.h>


BronzeCSNRPainter::BronzeCSNRPainter(MGKAbstractMonitor *parent) : MonitorPainter(parent)
{
    //    SweepStream *stream = SweepStreamContext::instance()->getSweepStream(MgkConstants::IE1SweepStream);
//    stream->listen();
//    connect(stream, &SweepStream::sweepReady, this, [this](QVector<double> sweep) {
//        input = sweep;
//    });

    m_handler = StreamConnector::instance()->bindToStream("epo1", [this](const QVariant& data){
        auto map = data.toMap();
        if(map["pressures"].canConvert<QVector<double>>()) {
            input = map["pressures"].value<QVector<double>>();
        }
    });

    TOatinuator.converter = [](QVariant val) -> double {
        switch (val.toInt()) {
        case 0:  return pow(0.3, 5);
        case 1:  return pow(0.3, 4);
        case 2:  return pow(0.3, 3);
        case 3:  return pow(0.3, 2);
        case 4:  return pow(0.3, 1);
        default: return 1.0;
        }
    };

    TOatinuator.gain = [this](double signal) -> double {
        return std::min(signal * gainTo * gainLevelMultiplier, limit);
    };

    connect(dynamic_cast<MGKBronzeRectMonitor*>(monitor()), &MGKBronzeRectMonitor::bronzeGainToChanged, this, &BronzeCSNRPainter::onGainTOChanged);

//    TOAtinuator.gainIn(&RU2Atinuator, &Amplifier::gainOut);
//    RU2Atinuator.gainIn(&RU1Atinuator, &Amplifier::gainOut);
//    RU1Atinuator.gainIn(&amplifier, &Amplifier::gainOut);

//    TOAtinuator.setPropertyConverter([](QVariant value) {
//        switch (value.toInt()) {
//        case 0: return std::pow(0.3, 5);
//        case 1: return std::pow(0.3, 4);
//        case 2: return std::pow(0.3, 3);
//        case 3: return std::pow(0.3, 2);
//        case 4: return std::pow(0.3, 1);
//        default: return 1.0;
//        }
//    });

//    TOAtinuator.setPropertyId("bronzeGainTO");
//    RU2Atinuator.setPropertyId("bronzeRU2");
//    RU1Atinuator.setPropertyId("bronzeRU1");
//    amplifier.setPropertyId("dispBronzeIOGain");
//    TOAtinuator.setGainLevelMultiplier(4);
}

void BronzeCSNRPainter::paint(QPainter *painter)
{
    MonitorPainter::paint(painter);
    Q_UNUSED(painter)
    auto pixels = reinterpret_cast<uint32_t*>(monitor()->drawImage.scanLine(0));
    int newI = static_cast<int>(KMath::instance()->dmap(bronzeSight, 0, 360, 0, input.size() - 1));
    const double minPressureRadius = monitor()->height() / 2 - 80;
    const double maxPressureRadius = monitor()->height() / 2 + 80;
    auto drawPoint = [=] (int i, bool leftToRight) {
        leftToRight = true;
        if(i >= input.size()) return;
        double intensifiedPressure = input[i];
        QVector<double> points = discretizer.calc(intensifiedPressure);
        for(int j = 0; j < points.size(); j++) {
            const double radius =  TOatinuator.gain(integrator.calc(points[leftToRight ? j : points.size() - j - 1]) * (maxPressureRadius - minPressureRadius)) + minPressureRadius;
            const double delta = -0.06;
            const double x = KMath::instance()->dmap(i * points.size() + (leftToRight ? j : points.size() - j - 1), 0, input.size() * points.size() - 1, 0, monitor()->width()) + delta;

            //surface->drawPoligon(static_cast<int>(x), static_cast<int>(surface->height() - radius));
            drawPrettyPoligon(pixels, static_cast<int>(x), static_cast<int>(monitor()->height() - radius));
        }
    };

    if(newI < input.size() && newI >= 0) {
        bronzeCurrentPressure = input[newI];
    }

    const bool leftToRight = true;
    if(newI == lastI) {
        drawPoint(newI, true);
    }
    else if(newI < lastI) {
        if(qAbs(newI - lastI) < input.size() / 2) {
            for(int i = newI; i < lastI; i++) {
                drawPoint(i, !leftToRight);
            }
        } else {
            for(int i = lastI; i < input.size(); i++) {
                drawPoint(i, leftToRight);
            }
            endPoligon();
            for(int i = 0; i < newI; i++) {
                drawPoint(i, leftToRight);
            }
            endPoligon();
        }
    } else {
        if(qAbs(newI - lastI) < input.size() / 2) {
            for(int i = lastI; i < newI; i++) {
                drawPoint(i, leftToRight);
            }
        } else {
            for(int i = lastI - 1; i >= 0; --i) {
                drawPoint(i, !leftToRight);
            }
            endPoligon();
            for(int i = input.size() - 1; i >= newI; --i) {
                drawPoint(i, !leftToRight);
            }
            endPoligon();
        }
    }
    lastI = newI;
}

void BronzeCSNRPainter::onGainTOChanged()
{
    gainTo = TOatinuator.converter(monitor()->property("bronzeGainTo"));
}
