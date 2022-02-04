#include "csnrpainter.h"
#include <QRandomGenerator>
#include <math/kmath.h>


CSNRPainter::CSNRPainter(MGKAbstractMonitor *parent) : MonitorPainter(parent)
{
    m_listener = StreamConnector::instance()->bindToStream("epo1", [this](const QVariant & value){
        auto map = value.toMap();
        input.append(map["pressures"].value<QVector<double>>());
    });
    elapsed.start();
}

void CSNRPainter::drawSight(uint32_t * pixels, double sight, double minRadius, double maxRadius) {
    const double angle = (sight - 90) * M_PI / 180;
    const double cos = std::cos(angle);
    const double sin = std::sin(angle);

    const int x1 = static_cast<int>(cos * minRadius + monitor()->width() / 2);
    const int y1 = static_cast<int>(sin * minRadius + monitor()->height() / 2);
    const int x2 = static_cast<int>(cos * maxRadius + monitor()->width() / 2);
    const int y2 = static_cast<int>(sin * maxRadius + monitor()->height() / 2);

    double brightness = sightBrightness;
    drawPrettyLine(pixels, x1, y1, x2, y2, brightness, 1, monitor()->color(), monitor()->backgroundColor());
}

void CSNRPainter::paint(QPainter *painter)
{
    auto pixels = reinterpret_cast<uint32_t*>(monitor()->drawImage.bits());
    auto ms = elapsed.elapsed();
    const double minPressureRadius = 80;
    const double maxPressureRadius = monitor()->width() / 4;
    auto drawPoint = [=] (int i) {
        if(i >= input.size()) return;
        double intensifiedPressure = input[i];
        QVector<double> points = discretizer.calc(intensifiedPressure);
        for(int j = 0; j < points.size(); j++) {
            const double radius = (integrator.calc(points[j]) * (maxPressureRadius - minPressureRadius) + minPressureRadius);
            const double delta = -0.06;
            const double angle = (KMath::dmap(i * points.size() + j, 0, input.size() * points.size() - 1, 0, 360) - 90) * M_PI / 180 + delta;

            const int px = static_cast<int>(std::cos(angle) * radius + m_imW * 0.5);
            const int py = static_cast<int>(std::sin(angle) * radius + m_imH * 0.5);

            drawPrettyPoligon(pixels, px, py);
        }
    };

    int newI = static_cast<int>(KMath::map(ms % 5000, 0, 5000, 0, input.size() - 1));

    if(newI < lastI) {
        for(int i = lastI; i < input.size(); i++) {
            drawPoint(i);
        }
        for(int i = 0; i < newI; i++) {
            drawPoint(i);
        }
    }
    for(int i = lastI; i < newI; i++) {
        drawPoint(i);
    }
    lastI = newI;

//    int currentSightPosition = static_cast<int>(KMath::dmap(sight, 0, 360, 0, input.size() - 1));
//    if(currentSightPosition >= 0 && currentSightPosition < input.size()) {
//        currentTargetPressure->setValue(input[currentSightPosition]);
//    }

//    drawSight(sight->value(), minPressureRadius, maxPressureRadius * 0.5, surface);

//    const auto & centerX = monitor()->width() / 2;
//    const auto & centerY = monitor()->height() / 2;
//    for (int i = 0; i < 360; ++i)
//    {
//        double rad = (i - 90) * M_PI / 180.;
//        if(i == std::clamp(i, lastI, lastI + points)) /*{*/
//            drawPixel(pixels, cos(rad) * (minPressureRadius + KMath::dmap(input[i % points], 0, 1, 0, maxPressureRadius - minPressureRadius)) + centerX, sin(rad) * minPressureRadius + centerY);
////        }
//        else
//            drawPixel(pixels, cos(rad) * (minPressureRadius) + centerX, sin(rad) * minPressureRadius + centerY);
//    }
//    (lastI += points) %= 360;

//    for(auto && input : input) {
//        input = QRandomGenerator::global()->generateDouble();
//    }

    monitor()->screenCursor()->paint(pixels);

    MonitorPainter::paint(painter);

}
