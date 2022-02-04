#ifndef MGKBRONZERECTMONITOR_H
#define MGKBRONZERECTMONITOR_H

#include "mgkrectmonitorprivate.h"

enum class BronzeMode {GLCycle, Search, RSGL, StepGL};
enum class BronzeDistanceScale {firstScale, secondScale, thirdScale, fourthScale};

class MGKBronzeRectMonitor : public MGKRectMonitorPrivate
{
    Q_OBJECT
    K_QML_TYPE(MGKBronzeRectMonitor)

    using BronzeGainTO = int;
    K_AUTO_PROPERTY(BronzeMode, bronzeMode, bronzeMode, setBronzeMode, bronzeModeChanged, BronzeMode::GLCycle)
    K_AUTO_PROPERTY(BronzeGainTO, bronzeGainTo, bronzeGainTo, setBronzeGainTo, bronzeGainToChanged, 0)
//    K_AUTO_PROPERTY(BronzeGainTO, bronzeGainTo, bronzeGainTo, setBronzeGainTo, bronzeGainToChanged, 0)
//    K_AUTO_PROPERTY(BronzeGainTO, bronzeGainTo, bronzeGainTo, setBronzeGainTo, bronzeGainToChanged, 0)

public:

    Q_ENUM(BronzeMode)
    Q_ENUM(BronzeDistanceScale)


    MGKBronzeRectMonitor(QQuickItem * parent = nullptr);

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter);

    // MGKRectMonitorPrivate interface
public slots:
    void onModeChanged();
};

#endif // MGKBRONZERECTMONITOR_H
