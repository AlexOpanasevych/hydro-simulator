#include "tucpainter.h"
#include <math.h>
//#include <src/lib/network/sweepstream.h>
#include <utility/propamplifier.h>
#include <math/kmath.h>
#include <QRandomGenerator>

TucPainter::TucPainter(MGKAbstractMonitor * parent) : MonitorPainter(parent) {

}

void TucPainter::paint(QPainter *painter) {
    Q_UNUSED(painter)

    uint32_t * pixels = reinterpret_cast<uint32_t*>(monitor()->drawImage.scanLine(0));

//    currentTime = (timer.elapsed() % maxTime) / 1000;

    double target = KMath::degreesDifference(currentTargetBearing, bearing);

    double radius = 0;
    double angle = 0;
    if(target < 5 && target > -5) {
        double radC = KMath::dmap(qAbs(target), 0, 5, 1, 0);
        radius = 100 * std::min(currentTargetPressure * radC * 2, 1.0);

        angle = KMath::dmap(target, -5, 5, 0, M_PI);

    } else {
        angle = QRandomGenerator::global()->generateDouble() * M_PI;
        radius = QRandomGenerator::global()->generateDouble() * 10;
    }

    int px = static_cast<int>(std::cos(angle) * radius);
    int py = static_cast<int>(std::sin(angle) * radius);

    //surface->drawLine(surface->centerX() - px, surface->centerY() - py, surface->centerY() + px, surface->centerY() + py);
    WuDrawLine(pixels, m_imW / 2 - px, m_imH / 2 - py, m_imH / 2 + px, m_imH / 2 + py, 1, monitor()->color());
//    WuDrawLine(pixels, m_imW / 2, m_imH / 2, 0, 0, 1, "#000000");

//    const int spacing = static_cast<int>(monitor()->height() / (linesCount - 1));
//    const int margin = static_cast<int>((monitor()->height() - (spacing * (linesCount - 1))) / 2);
//    const int x1 = static_cast<int>(margin + spacing);
//    const int x2 = static_cast<int>(monitor()->width() - margin - spacing);

//    drawPrettyLine(x1, monitor()->height() / 2, x2, monitor()->height() / 2, painter, 1, 1, lineColor);
//    int startMargin = margin;
//    double cx = monitor()->width() / 2;
//    double cy = monitor()->height() / 2;
//    for(int i = 0; i < linesCount; i++) {
//        auto lineLength = std::sqrt(std::pow(monitor()->width() / 2, 2) - std::pow(QLineF(cx, cy, startMargin, cy).length(), 2));
//        double yHigh = cy - lineLength;
//        double yLow = cy + lineLength;
//        drawPrettyLine(startMargin, yHigh, startMargin, yLow, painter, 1, 1, lineColor);
//        startMargin+=spacing;
//    }

//    drawDashedLine(x1, cy - yDeviation, x2, cy + yDeviation, painter, 2, 5, lineColor);

//    double target = KMath::instance()->degreesDifference(currentTargetBearing, bearing);

//    //double radius = KMath::getInstance()->dmap(qAbs(target), 5, 0, 0, 100 * std::min(currentTargetPressure->value() * 2, 1.0));
//    double radius = 0.0, angle = 0.0;
//    if(target < 5 && target > -5) {
//        double radC = KMath::dmap(qAbs(target), 0, 5, 1, 0);
//        radius = 100 * std::min(currentTargetPressure * radC * 2, 1.0);

//        angle = KMath::dmap(target, -5, 5, 0, M_PI);

//    } else {
//        angle = QRandomGenerator::global()->generateDouble() * M_PI;
//        radius = QRandomGenerator::global()->generateDouble() * 10;
//    }

//    int px = static_cast<int>(std::cos(angle) * radius);
//    int py = static_cast<int>(std::sin(angle) * radius);

//    //surface->drawLine(surface->centerX() - px, surface->centerY() - py, surface->centerY() + px, surface->centerY() + py);
//    drawPrettyLine(monitor()->width() / 2 - px, monitor()->height() / 2 - py, monitor()->height() / 2 + px, monitor()->height() / 2 + py, painter, 1, 1);
}
