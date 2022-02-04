#include "epo1painter.h"

#include <QTimer>

EPO1Painter::EPO1Painter(MGKAbstractMonitor * parent) : SweepPainter(parent) {
//    spatialChannels.resize(groupCount());
//    for(auto && ch : spatialChannels) {
//        ch.freqChannels.resize(3);
//        for(auto && freqChannel : ch.freqChannels) {
//            freqChannel = 0.8;
//        }
//    }
//    setReverberationFactor(50);

//    spatialProcessor->setMode(XnPressureConvertor::SpatialProcessing);
//    spatialProcessor->setChannalCount(3);
//    spatialProcessor->setMinVelocity(math->fromKnots(-15));
//    spatialProcessor->setMaxVelocity(math->fromKnots(15));

//    KBus::personalHandler("platinaWaveRecieved", this, [this](QVariant value) {
//        QVariantMap object = value.toMap();
//        double angle = object["angle"].toDouble();
//        double emissionTime = object["emissionTime"].toDouble();
//        double intensity = object["intensity"].toDouble();
//        double radialVelocity = object["radialVelocity"].toDouble();
//        spatialProcessor->addPressure(angle, intensity * platina5AssistGain(), radialVelocity, emissionTime);
//    });

//    connect(spatialProcessor, &XnPressureConvertor::pressureAdded, this, [](QVariant value) {
//        KBus::instance()->push("platinaProcessedSignal", value);
//    });

//    KMonoObserver::bindTo(this)
//            .exec(spatialProcessor, &XnPressureConvertor::setXnCount)
//            .observe("xnCount");

//    KMonoObserver::bindTo(this)
//            .exec(spatialProcessor, &XnPressureConvertor::setXnShiftAngle)
//            .observe("xnShiftAngle");

//    KMonoObserver::bindTo(this)
//            .exec(spatialProcessor, &XnPressureConvertor::setXnRotation)
//            .observe("xnRotation");

//    KMonoObserver::bindTo(this)
//            .exec([this](QVariant value) {
//        spatialProcessor->setNoiseAmplitude(value.toDouble() / 0.2);
//    })
//            .observe("noiseCoeficient");

}

//void EPO1Painter::paint(QPainter *painter) {
//    MonitorPainter::paint(painter);
//    const double c = KMath::dmap(monitor()->soundSpeed() * monitor()->impulseTime(), 0, monitor()->maxDistance(), 0, 1);
//    paintSweep(c, monitor()->maxDistance(), painter);

//    int px;
//    if(monitor()->boardMode() == BoardMode::LEFT) {
//        px = static_cast<int>(KMath::dmap(sight, rightBoardStart(), rightBoardEnd(), getLeftGroupCenter(), getRightGroupCenter()));
//    } else {
//        px = static_cast<int>(KMath::dmap(sight, leftBoardStart(), leftBoardEnd(), getLeftGroupCenter(), getRightGroupCenter()));
//    }
//    paintSight(px, sightBrightness, monitor()->distance(), monitor()->maxDistance(), painter);

//    QPointF startPoint(startEndMargin(), monitor()->height() - topBottomMargin());
//    const auto tracksCount = groupCount() * 2 * tracksInGroup();
//    const auto spacing = (monitor()->width() - 2 * startEndMargin()) / (tracksCount - 1); // 2 sides, 6 tracks in group
//    int i = tracksCount;
//    const auto distToEnd = (tracksCount - 1) * spacing;
//    const auto t = tan(KMath::degreesToRadians(5));
//    const auto startHeight = distToEnd * t;
//    qDebug() << spacing << "spacing";
//    for(auto channe : spatialChannels) {
//        for(auto freqChannel : channe.freqChannels) {
//            drawPrettyLine(startPoint.x(), startPoint.y() - startHeight * i / tracksCount, startPoint.x(), startPoint.y() - monitor()->height() + 2 * topBottomMargin(), painter, freqChannel, 2);
//            startPoint.setX(startPoint.x() + spacing);
//            i--;
//            drawPrettyLine(startPoint.x(), startPoint.y() - startHeight * i / tracksCount, startPoint.x(), startPoint.y() - monitor()->height() + 2 * topBottomMargin(), painter, freqChannel / 2, 2);
//            startPoint.setX(startPoint.x() + spacing);
//            i--;
//        }
//    }
//}
