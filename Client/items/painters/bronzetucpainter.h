#ifndef BRONZETUCPAINTER_H
#define BRONZETUCPAINTER_H

//#include <src/lib/utility/karraymemory.h>
//#include <src/lib/utility/krangedistribution.h>

#include "monitorpainter.h"

#include <math/kmath.h>
#include <items/mgkabstractmonitor.h>
#include <utility/kintegrator.h>
#include <utility/krangedistribution.h>
#include <utility/propamplifier.h>
#include <streamconnector.h>

class BronzeTucPainter : public MonitorPainter {

    enum class BronzeMode {CicleGLMode = 0, SearchMode, RSMode, RSGLMode, StepMode, CircleStopMode, IdleMode};
    enum class ModeInput { CicleGL = 0, Search, RS, Step, CircleStop, Empty };
    enum class SubMode { APG = 0, PA, EK };

    K_AUTO_PROPERTY(double, bronzeCurrentPressure, bronzeCurrentPressure, setBronzeCurrentPressure, bronzeCurrentPressureChanged, 0.0)
    K_AUTO_PROPERTY(double, currentTargetBearing, currentTargetBearing,   setCurrentTargetBearing, currentTargetBearingChanged, 0.0)
    K_AUTO_PROPERTY(double, bronzeSight, bronzeSight, bronzeSight,        setBronzeSightChanged, 0.0)
    K_AUTO_PROPERTY(double, bronzeBearing, bronzeBearing,                 setBronzeBearing, bronzeBearingChanged, 0.0)
    K_AUTO_PROPERTY(double, bronzeXnShiftAngle, bronzeXnShiftAngle,       setBronzeXnShiftAngle, bronzeXnShiftAngleChanged, 0.0)
    K_AUTO_PROPERTY(double, bronzeXnRotation, bronzeXnRotation,           setBronzeXnRotation, bronzeXnRotationChanged, 0.0)
    K_AUTO_PROPERTY(int,    bronzeXnCount, bronzeXnCount,                 setBronzeXnCount, bronzeXnCountChanged, 0)
    K_AUTO_PROPERTY(double, ownCourse, ownCourse,                         setOwnCourse, ownCourseChanged, 0.0)
    K_AUTO_PROPERTY(int,    bronzeSelectedChannel, bronzeSelectedChannel, setBronzeSelectedChannel, bronzeSelectedChannelChanged, 0.0)
    K_AUTO_PROPERTY(double, bronzeTPAssistGain, bronzeTPAssistGain,       setBronzeTPAssistGain, bronzeTPAssistGainChanged, 0.0)
    K_AUTO_PROPERTY(double, noise, noise, setNoise, noiseChanged, 0.0)



    K_AUTO_PROPERTY(BronzeMode, bronzeMode, bronzeMode, bronzeMode, bronzeModeChanged, BronzeMode::CicleGLMode)
    QVector<QPair<double, double>> targets;
//    KMath *math = KMath::instance();
    double lastTargetBearing = 0;


//    PropAmplifier amplifier;
//    PropAmplifier RU1Atinuator;
//    PropAmplifier RU2Atinuator;
//    PropAmplifier TOAtinuator;

//    KIntegrator tucNoiseIntegrator;
//    KRangeDistribution rangeDistribution;
    listener m_handler = nullptr;
//    KArrayMemory arrayMemory;
//    KArrayMemory tucMemory;

    void doplerProcessorInit();
public:
    BronzeTucPainter(MGKAbstractMonitor * monitor);

    // AbstractMonitorPainter interface
public:
    void paint(QPainter *painter) override;
};

#endif // BRONZETUCPAINTER_H
