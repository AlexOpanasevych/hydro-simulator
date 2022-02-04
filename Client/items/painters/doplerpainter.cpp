#include "doplerpainter.h"
#include <math/kmath.h>
#include "../mgkabstractmonitor.h"
#include <QTimer>
#include <math.h>


DoplerPainter::DoplerPainter(MGKAbstractMonitor * parent) : SweepPainter(parent)
{
    m_handler = StreamConnector::instance()->bindToStream("dopler", [this](const QVariant& data){
        qDebug() << "sweeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeep";
        auto map = data.toMap();

        qDebug() << map["distance"];
        qDebug() << map["pressure"];
        qDebug() << map["xz"];

        if(map["sweep"].canConvert<QVector<double>>()) {
            pushData(map["time"].toDouble(), map["sweep"].value<QVector<double>>());
        } else qDebug() << "wrong data coming from stream";

    });

    setGroupCount(15);
    setTracksInGroup(4);
    setTrackWidth(4);
//    setTrackSpacing(4);
    setGroupSpacing(6);
//    setReverberationFactor(50);
//    spatialChannels.resize(groupCount());
//    for(auto && ch : spatialChannels) {
//        ch.freqChannels.resize(1);
//        for(auto && freqChannel : ch.freqChannels) {
//            freqChannel = 0.8;
//        }
//    }
}

//void DoplerPainter::paint(QPainter *painter)
//{
//    SweepPainter::paint(painter);
//    QPointF startPoint(startEndMargin(), monitor()->height() - topBottomMargin());
//    const auto tracksCount = groupCount() * tracksInGroup();
//    const auto spacing = (monitor()->width() - 2 * startEndMargin()) / (tracksCount - 1); // 2 sides, 6 tracks in group
//    int i = tracksCount;
//    const auto distToEnd = (tracksCount - 1) * spacing;
//    const auto t = tan(KMath::degreesToRadians(5));
//    const auto startHeight = distToEnd * t;
//    qDebug() << spacing << "spacing";
//    for(auto channe : spatialChannels) {
//        for(auto freqChannel : channe.freqChannels) {
//            drawPrettyLine(startPoint.x(), startPoint.y()/* - startHeight * i / tracksCount*/, startPoint.x(), startPoint.y() - monitor()->height() + 2 * topBottomMargin(), painter, freqChannel, 6);
//            startPoint.setX(startPoint.x() + spacing);
//            i--;
//            drawPrettyLine(startPoint.x(), startPoint.y()/* - startHeight * i / tracksCount*/, startPoint.x(), startPoint.y() - monitor()->height() + 2 * topBottomMargin(), painter, freqChannel, 4);
//            startPoint.setX(startPoint.x() + spacing);
//            i--;
//            drawPrettyLine(startPoint.x(), startPoint.y()/* - startHeight * i / tracksCount*/, startPoint.x(), startPoint.y() - monitor()->height() + 2 * topBottomMargin(), painter, freqChannel / 2, 2);
//            startPoint.setX(startPoint.x() + spacing);
//            i--;
//            drawPrettyLine(startPoint.x(), startPoint.y()/* - startHeight * i / tracksCount*/, startPoint.x(), startPoint.y() - monitor()->height() + 2 * topBottomMargin(), painter, freqChannel / 2, 2);
//            startPoint.setX(startPoint.x() + spacing);
//            i--;
//        }
//    }



//    paintGradientSweep(0, monitor()->maxDistance(), painter);
//    const int px = static_cast<int>(KMath::dmap(doplerChannels(), 0, 14, getLeftGroupCenter(), getRightGroupCenter()));
//    paintSight(px + trackWidth() + trackSpacing(), rightSightBrightness(), monitor()->distance(), monitor()->maxDistance(), painter);
//}

//void DoplerPainter::onSweepReady(QVector<double> sweep) {
//    array.push_back(m_currentDistance - sweep[sweep.size() - 1]);
//    sweep[sweep.size() - 1] = m_currentDistance - KMath::instance()->customContainerAverage(array);
//    QVector<QVector<double>> discretizedSweep = discretizer.calc(sweep);
//    for(QVector<double> & s : discretizedSweep) {
//        pushData(s);
//    }

//    //auto func = KStaticFunction(this, &DoplerPainter::onSweepReady);
//    //auto fff = KStaticFunction(aaa);

//    //auto fff2 = KStaticFunction([](){});
//}

//void DoplerPainter::onSpChannelCntChanged(int spChannelCnt)
//{
//    spatialChannels.resize(spChannelCnt);

//    for(auto ch : spatialChannels) {
//        for(auto freqCh : ch.freqChannels) {
//            freqCh = 0.8;
//        }
//    }

//}
