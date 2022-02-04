#include "sweeppainter.h"

#include <math/kmath.h>

#include <items/monitors/mgkrectmonitorprivate.h>

#include <QRandomGenerator>
#include <QRgb>
#include <math.h>
#include <QPainter>


SweepPainter::SweepPainter(MGKAbstractMonitor* monitor) : MonitorPainter(monitor) {

    qRegisterMetaType<QVector<double>>();
    qRegisterMetaTypeStreamOperators<QVector<double>>();


    connect(this, &SweepPainter::imHChanged, this, &SweepPainter::imSizeChanged);
    connect(this, &SweepPainter::imHChanged, this, [](int imH){
        qDebug() << "imH changed" << imH;
    });
    connect(this, &SweepPainter::imWChanged, this, &SweepPainter::imSizeChanged);
    connect(this, &SweepPainter::imWChanged, this, [](int imW){

        qDebug() << "imW changed" << imW;
    });

    connect(monitor, &MGKAbstractMonitor::epModeChanged, this, &SweepPainter::proceedEpModeChanged);

    connect(monitor, &MGKAbstractMonitor::searchScaleChanged, this, &SweepPainter::proceedEpModeChanged);


    connect(this, &SweepPainter::yShiftChanged, this, [](double yShift){
        qDebug() << "yShift changed" << yShift;
    });
    connect(this, &SweepPainter::imSizeChanged, this, &SweepPainter::proceedEpModeChanged);

    connect(monitor, &MGKAbstractMonitor::searchAngleModeChanged, this, &SweepPainter::proceedEpModeChanged);
    connect(monitor, &MGKAbstractMonitor::boardModeChanged, this, &SweepPainter::proceedEpModeChanged);
    connect(monitor, &MGKAbstractMonitor::searchModeChanged, this, &SweepPainter::proceedEpModeChanged);

    proceedEpModeChanged();

    m_listener = StreamConnector::instance()->bindToStream("epo1", [this](const QVariant & value){
        auto map = value.toMap();
//        soundSpeed = map["angle"].toDouble();
//        impulseTime = map["emissionTime"].toDouble();
//        intensity = map["intensity"].toDouble();
//        xnRotation = map["xnRotation"].toDouble();
//        board = map["radialVelocity"].toInt();
//        sight = map["sight"].toDouble();
//        distance = map["distance"].toDouble();
//        qDebug() << "data incomming" << value;
        if(this->monitor()->monitorWork()) pushData(map["time"].toDouble(), map["pressures"].value<QVector<double>>());
    });
    timer.start();


}

//void SweepPainter::paintSight(int px, double sightBrightness, double distance, double maxDistance, QPainter *painter) {
//    int py = static_cast<int>(KMath::instance()->dmap(distance, 0, maxDistance, monitor()->height(), 0));
//    if(sightBrightness > 0.1) {
//        setPixel(px, py, painter, sightBrightness);
//        drawCircle(px, py, 1, painter, sightBrightness);
//        drawCircle(px, py, 2, painter, sightBrightness);
//    }
//}


void SweepPainter::drawDataLine(double angle, double xPos, double yPos, const QVector<double> &pressures, int trackWidth, int trackShift, int groupShift, int tracksInGroup, uint32_t * pixels, double pressureToAlphaKoeff, QColor color)
{
//    qDebug() << color.name();
//    auto imW = image->width(), imH = image->height();
//    QColor c;
//    auto drawPixel = [pixels, imW, imH, c](int x, int y, double alpha) {
//        if (x >= 0 && y >= 0 && x < imW && y < imH/* && parent->mask.pixel(x, y) != 0*/) {
//            pixels[(y * imW) + x] = qRgba(c.red(), c.green(), c.blue(), alpha);
//        }
//    };

    int x = xPos;
    double y = yPos;
    double dy = tan(angle);
    for (int i = 0; i < pressures.size(); ++i) {
        //draw track data line from x to x +trackWidth
        drawSweepLine(pixels, x, y, x + trackWidth, y + dy * trackWidth, pressures[i]*pressureToAlphaKoeff, color);
//        drawPrettyLine(pixels, x, y, x + trackWidth, y + dy * trackWidth, pressures[i]*pressureToAlphaKoeff, 1, color, backgroundColor);
        //shift x pos
        if ((i+1) % tracksInGroup == 0) { // group shift
            x += trackWidth + groupShift;
            y += dy * (trackWidth + groupShift);
        } else {
            x += trackWidth;
            y += dy * (trackWidth + trackShift);
            x += trackShift;
        }
    }
}

void SweepPainter::drawSweepLine(uint32_t *pixels, double x0, double y0, double x1, double y1, double alpha, const QColor &color)
{
    auto ipart = [](double x) -> int {return int(std::floor(x));};
    auto round = [](double x) -> double {return std::round(x);};
    auto fpart = [](double x) -> double {return x - std::floor(x);};
    auto rfpart = [=](double x) -> double {return 1 - fpart(x);};

    const bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        std::swap(x0,y0);
        std::swap(x1,y1);
    }
    if (x0 > x1) {
        std::swap(x0,x1);
        std::swap(y0,y1);
    }

    const float dx = x1 - x0;
    const float dy = y1 - y0;
    const float gradient = (dx == 0) ? 1 : dy/dx;

    int xpx11;
    float intery;
    {
        const float xend = round(x0);
        const float yend = y0 + gradient * (xend - x0);
        const float xgap = rfpart(x0 + 0.5);
        xpx11 = int(xend);
        const int ypx11 = ipart(yend);
        if (steep) {
            drawPixel(pixels, ypx11,     xpx11, rfpart(yend) * xgap * alpha, color);
            drawPixel(pixels, ypx11 + 1, xpx11,  fpart(yend) * xgap * alpha, color);
        } else {
            drawPixel(pixels, xpx11, ypx11,    rfpart(yend) * xgap * alpha, color);
//            drawPixel(pixels, xpx11, ypx11 + 1, fpart(yend) * xgap * alpha, color);
        }
        intery = yend + gradient;
    }

    int xpx12;
    {
        const float xend = round(x1);
        const float yend = y1 + gradient * (xend - x1);
        const float xgap = rfpart(x1 + 0.5);
        xpx12 = int(xend);
        const int ypx12 = ipart(yend);
        if (steep) {
            drawPixel(pixels, ypx12,     xpx12, rfpart(yend) * xgap * alpha, color);
            drawPixel(pixels, ypx12 + 1, xpx12,  fpart(yend) * xgap * alpha, color);
        } else {
            drawPixel(pixels, xpx12, ypx12,    rfpart(yend) * xgap * alpha, color);
//            drawPixel(pixels, xpx12, ypx12 + 1, fpart(yend) * xgap * alpha, color);
        }
    }

    if (steep) {
        for (int x = xpx11 + 1; x < xpx12; x++) {
            drawPixel(pixels, ipart(intery),     x, rfpart(intery) * alpha, color);
            drawPixel(pixels, ipart(intery) + 1, x,  fpart(intery) * alpha, color);
            intery += gradient;
        }
    } else {
        for (int x = xpx11 + 1; x < xpx12; x++) {
            drawPixel(pixels, x, ipart(intery),     rfpart(intery) * alpha, color);
//            drawPixel(pixels, x, ipart(intery) + 1,  fpart(intery) * alpha, color);
            intery += gradient;
        }
    }
}

//int SweepPainter::paintSweep(double incline, double maxDistance, QPainter * painter) {
//    int x = startPosition;
//    if(!pressuresQueue.isEmpty() && (monitor()->width() != 0) && (monitor()->height() != 0)) {
//        while (!pressuresQueue.isEmpty()) {
//            x = startPosition;
//            auto pressures = pressuresQueue.dequeue();
//            signed y = static_cast<signed>(KMath::dmap(pressures.last(), 0, maxDistance, 0, monitor()->height()));
//            if(y < lastY) {
//                lastY = y;
//                break;
//            } else {
//                lastY = y;
//            }

//            for(int g = 0; g < groupCount(); ++g) {
//                for(int t = 0; t < tracksInGroup(); ++t) {
//                    auto pressure = pressures[std::min(g * tracksInGroup() + t, pressures.size() - 1)];
//                    if(!qFuzzyCompare(m_reverberationFactor, 0)) {
//                        pressure += KMath::gaus(y / m_reverberationFactor);
//                    }

//                    if(showMultiSight()) {
//                        QVector<double> values = sightYDiscretizer.calc(KMath::dmap(multiSightDistance(), 0, maxDistance, monitor()->height(), 0));
//                        for(double val : values) {
//                            setPixel(x, static_cast<int>(val), painter, multiSightBrightness());
//                        }
//                    }

//                    for (int p = 0; p < trackWidth(); ++p) {
//                        int safeY = static_cast<int>(monitor()->height() - (y - (x - monitor()->width()) * incline));
//                        safeY = qMin((double)safeY, monitor()->height() - 1);
//                        safeY = qMax(safeY, 0);
//                        setPixel(x, safeY, painter, pressure);
//                        x++;
//                    }
//                    x += trackSpacing();
//                }
//                x += groupSpacing();
//            }
//        }
//    }
//    setSweepEnd(x);
//    return 0;
//}

//int SweepPainter::paintGradientSweep(double incline, double maxDistance, QPainter *painter) {
//    int x = startPosition;
//    if((monitor()->width() != 0) && (monitor()->height() != 0) && (tracksInGroup() != 0)) {
//        while (!pressuresQueue.isEmpty()) {
//            x = startPosition;
//            auto pressures = pressuresQueue.dequeue();
//            double gradientStep = 1.0 / static_cast<double>(tracksInGroup());
//            signed y = static_cast<signed>(KMath::instance()->dmap(pressures.last(), 0, maxDistance, 0, monitor()->height()));
//            if(y < lastY) {
//                lastY = y;
//                break;
//            } else {
//                lastY = y;
//            }

//            for(int g = 0; g < groupCount(); ++g) {
//                double pressure = pressures[std::min(g, pressures.size() - 1)];
//                if(!qFuzzyCompare(m_reverberationFactor, 0)) {
//                    pressure += KMath::instance()->gaus(y / m_reverberationFactor);
//                }
//                for(int t = 0; t < tracksInGroup(); ++t) {
//                    double currentTrackPressure = pressure * gradientStep * (tracksInGroup() - t);
//                    for (int p = 0; p < trackWidth(); ++p) {
//                        int yy = static_cast<int>(monitor()->height() - (y - (x - monitor()->width()) * incline));
//                        yy = qMin((double)yy, monitor()->height() - 1);
//                        yy = qMax(yy, 0);
//                        setPixel(x, yy, painter, currentTrackPressure);
//                        x++;
//                    }
//                    x += trackSpacing();
//                }
//                x += groupSpacing();
//            }
//        }
//    }
//    setSweepEnd(x);
//    return getSweepEnd();
//}

SweepPainter::DataLine SweepPainter::popData() {
    return avaliableLines.dequeue();
}

bool SweepPainter::hasData() {
    return !avaliableLines.isEmpty();
}

void SweepPainter::proceedModeChange(){

    if(m_imW > 0 && m_imH > 0) {
        switch(this->monitor()->epMode()){
        case EpO1Mode::O1:
            switch(this->monitor()->searchMode()) {
            case SearchMode::DPBP:
                [[fallthrough]];
            case SearchMode::DP:
                switch(this->monitor()->searchAngleMode()) {
                case SearchAngleMode::Search120:
                    startPosition = startEndMargin();
                    endPosition = m_imW - startEndMargin();
                    break;
                case SearchAngleMode::Search60:
                    switch(this->monitor()->boardMode()) {
                    case BoardMode::LEFT:
                        startPosition = startEndMargin();
                        endPosition = m_imW / 2 - startEndMargin();
                        break;
                    case BoardMode::RIGHT:
                        startPosition = m_imW / 2 + startEndMargin();
                        endPosition = m_imW - startEndMargin();
                        break;

                    }
                    break;
                }
                break;
            case SearchMode::BP:
                startPosition = startEndMargin();
                endPosition = m_imW - startEndMargin();
                break;
            }
            break;
        case EpO1Mode::O2:
            startPosition = startEndMargin();
            endPosition = m_imW - startEndMargin();
            break;
        case EpO1Mode::S1:
            [[fallthrough]];
        case EpO1Mode::S2:
            startPosition = m_imW / 4 + startEndMargin();
            endPosition = m_imW / 4 * 3 - startEndMargin();
            break;
        case EpO1Mode::Shp:
            break;
        }
//        calculateSpacings();
        auto tann = tan(KMath::degreesToRadians(this->angle()));
        auto yShiftt = tann * /*(groupCount() * (groupSpacing() + trackSpacing() * (tracksInGroup() - 1)))*/(endPosition - startPosition);
        setYShift(topBottomMargin() + yShiftt);
        qDebug() << "setting yshift =" << yShift() << endPosition - startPosition;
    }
}

void SweepPainter::proceedEpModeChanged(){
    switch(this->monitor()->epMode()) {
    case EpO1Mode::O1:
        switch (this->monitor()->searchMode()) {
        case SearchMode::DPBP:

            [[fallthrough]];
        case SearchMode::DP:
            switch(this->monitor()->searchAngleMode()) {
            case SearchAngleMode::Search60:

                setGroupCount(6);
                setTracksInGroup(6);
                break;
            case SearchAngleMode::Search120:
                setGroupCount(12);
                setTracksInGroup(6);
                break;

            }
            break;
        case SearchMode::BP:
            setGroupCount(32);
            setTracksInGroup(2);
            setAngle(0);
            break;
        }
        initEPO1Spacings();
        break;
    case EpO1Mode::O2:
        setGroupCount(15);
        setTracksInGroup(4);
        initEPO2Spacings();
        break;
    case EpO1Mode::Shp:
        break;
    case EpO1Mode::S1:
        [[fallthrough]];
    case EpO1Mode::S2:

        setGroupCount(6);
        setTracksInGroup(6);
        initEPS1S2Spacings();
        break;
    }
    proceedScaleChanged();
    proceedModeChange();
}

void SweepPainter::proceedScaleChanged(){
    switch(this->monitor()->epMode()) {
    case EpO1Mode::O1:
        switch(this->monitor()->searchMode()) {
        case SearchMode::DPBP:
            [[fallthrough]];
        case SearchMode::DP:
            switch (this->monitor()->searchScale()) {
            case SearchScale::Scale1:
                setAngle(10);
                break;
            case SearchScale::Scale2:
                setAngle(15);
                break;
            case SearchScale::Scale3:
                setAngle(20);
                break;
            case SearchScale::Scale4:
                setAngle(25);
                break;

    //        case SearchScale::ZeroScale:
    //            setAngle(0);
    //            break;
            }
            break;
        case SearchMode::BP:
            setAngle(0);
            break;
        }
        break;
    case EpO1Mode::O2:
        setAngle(0);
        break;
    case EpO1Mode::S1:
        [[fallthrough]];
    case EpO1Mode::S2:
        setAngle(0);
        break;
    case EpO1Mode::Shp:
        break;
    }
    qDebug() << "angle changed" << angle();
}

void SweepPainter::paint(QPainter *painter)
{
    Q_UNUSED(painter)

    if(!monitor()->monitorWork())
        return;


    auto pixels = reinterpret_cast<uint32_t*>(monitor()->drawImage.scanLine(0));
//    drawPrettyLine(pixels, startEndMargin(), 300, m_imW - startEndMargin(), 300 + tan(KMath::degreesToRadians(angle())) * m_imW - startEndMargin(), 1, 1);

    currentTime = timer.elapsed() / 1000.;
    auto angle = KMath::degreesToRadians(this->angle());

    while (!avaliableLines.isEmpty()) {
        auto& dl = avaliableLines.front();
        if (dl.time > currentTime) break;
        dl.pressures.resize(m_groupCount * m_tracksInGroup);
        double yPos = KMath::dmap(dl.time * monitor()->soundSpeed(), 0, monitor()->maxDistance(), monitor()->height() - m_yShift, -m_yShift);
        painter->setPen(QPen(monitor()->color()));
        painter->drawText(10, 10, QString::number(yPos, 'f', 2) + " " + QString::number(m_yShift));
        drawDataLine(angle,
                     startPosition,
                     yPos,
                     dl.pressures,
                     m_trackWidth,
                     m_trackSpacing,
                     m_groupSpacing,
                     m_tracksInGroup,
                     pixels,
                     monitor()->brightness(),
                     monitor()->color());
        avaliableLines.dequeue();
    }
    monitor()->screenCursor()->paint(pixels);

    MonitorPainter::paint(painter);
}

void SweepPainter::pushData(double time, const QVector<double> & pressures) {
    avaliableLines.enqueue({.time = time, .pressures = pressures});
}

int SweepPainter::getStartPosition() const { return startPosition; }
void SweepPainter::setStartPosition(int value) { startPosition = value; }

void SweepPainter::calculateSpacings()
{
    assert (endPosition >= startPosition && "End position must be lower than start position");


    const auto length = endPosition - startPosition;

    const auto groupSplength = (m_groupCount-1) * m_groupSpacing;

    const auto trackSpace = (m_trackWidth * m_tracksInGroup + m_trackSpacing * (m_tracksInGroup - 1)) * m_groupCount;
    const auto availableSpace = length - trackSpace - groupSplength;
    setTrackWidth(m_trackWidth + availableSpace / (m_tracksInGroup * m_groupCount) - 1);

    qDebug() << "setting track width" << trackWidth();
    qDebug() << "setting group spacing" << m_groupSpacing;
    qDebug() << "setting track spacing" << m_trackSpacing;
//    while(groupSplength > availableGroupSpace) {
//         setGroupSpacing(m_groupSpacing-1);
//         qDebug() << "groupSpacing is too big, trying to set groupSpacing - 1, groupSpacing =" << m_groupSpacing;
//    }
    //    assert(length >= );
}

void SweepPainter::initEPO1Spacings()
{
    setStartEndMargin(12);
    setGroupSpacing(6);
    setTrackSpacing(3);
    setTrackWidth(4);
}

void SweepPainter::initEPO2Spacings()
{
    setStartEndMargin(12);
    setGroupSpacing(7);
    setTrackSpacing(4);
    setTrackWidth(4);
}

void SweepPainter::initEPS1S2Spacings()
{

}

