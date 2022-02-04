#ifndef SWEEPPAINTER_H
#define SWEEPPAINTER_H

#include "monitorpainter.h"

#include <utility/kdiscretizer.h>

#include <QElapsedTimer>
#include <QQueue>
#include <QTimer>
#include <kmacro.h>
#include <streamconnector.h>


class SweepPainter : public MonitorPainter {
    Q_OBJECT


    struct DataLine{
        double time;
        QVector<double> pressures;
    };


    QQueue<DataLine> avaliableLines;
    double currentTime = 0;
    K_AUTO_PROPERTY(int, groupCount, groupCount, setGroupCount, groupCountChanged, 12)
    K_AUTO_PROPERTY(int, startEndMargin, startEndMargin, setStartEndMargin, startEndMarginChanged, 4)
    K_AUTO_PROPERTY(double, angle, angle, setAngle, angleChanged, 15)
    K_AUTO_PROPERTY(int, topBottomMargin, topBottomMargin, setTopBottomMargin, topBottomMarginChanged, 6)
//    K_CONST_PROPERTY(int, maxGroupCount, 15)
    K_AUTO_PROPERTY(int, tracksInGroup, tracksInGroup, setTracksInGroup, tracksInGroupChanged, 3)
    K_AUTO_PROPERTY(int, trackWidth, trackWidth, setTrackWidth, trackWidthChanged, 4)
    K_READONLY_PROPERTY(int, trackSpacing, trackSpacing, setTrackSpacing, trackSpacingChanged, 3)
    K_AUTO_PROPERTY(int, groupSpacing, groupSpacing, setGroupSpacing, groupSpacingChanged, 6)
    K_READONLY_PROPERTY(double, yShift, yShift, setYShift, yShiftChanged, 0.0)

    QElapsedTimer timer;

    listener m_listener = nullptr;
    QTimer * pushTimer = new QTimer(this);

protected:
    int startPosition = startEndMargin();
    int endPosition;

public:
    SweepPainter(MGKAbstractMonitor* monitor);
    void pushData(double time, const QVector<double> &pressures);
//    QVector<SpatialChannel> spatialChannels;
    int getStartPosition() const;
    void setStartPosition(int value);
//    int getSweepEnd() const;

//    int getLeftGroupCenter() const;
//    int getRightGroupCenter() const;
//    double reverberationFactor() const;
//    void setReverberationFactor(double reverberationFactor);

//    bool showMultiSight() const;
//    void setShowMultiSight(bool showMultiSight);

//    double multiSightDistance() const;
//    void setMultiSightDistance(double multiSightDistance);

//    double multiSightBrightness() const;
//    void setMultiSightBrightness(double multiSightBrightness);

    void calculateSpacings();

    inline void initEPO1Spacings();
    inline void initEPO2Spacings();
    inline void initEPS1S2Spacings();


signals:
    void imSizeChanged();
protected:
//    void paintSight(int px, double sightBrightness, double distance, double maxDistance, QPainter * painter);
//    void paintGroup();
//    int paintSweep(double incline, double maxDistance, QPainter * painter);
//    int paintGradientSweep(double incline, double maxDistance, QPainter * painter);
    void drawDataLine(double angle, double xPos, double yPos, const QVector<double> & pressures, int trackWidth, int trackShift, int groupShift, int tracksInGroup, uint32_t *pixels, double pressureToAlphaKoeff, QColor color = "#ffff00");
    void drawSweepLine(uint32_t * pixels, double x0, double y0, double x1, double y1, double alpha, const QColor & color);

    DataLine popData();
    bool hasData();

    // MonitorPainter interface

public slots:
    void proceedModeChange();
    void proceedEpModeChanged();
    void proceedScaleChanged();
public:
    void paint(QPainter *painter);
};

//Q_DECLARE_METATYPE(QVector<double>)

#endif // SWEEPPAINTER_H
