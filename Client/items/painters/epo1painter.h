#ifndef EPO1PAINTER_H
#define EPO1PAINTER_H

#include "sweeppainter.h"
#include <kmacro.h>
#include <math/kmath.h>
#include <utility/kvectordiscretizer.h>
#include <streamconnector.h>

class EPO1Painter : public SweepPainter
{
//    double currentDistance, currentDistance, setCurrentDistance, currentDistanceChanged, 0.0)
//    int xnCount, xnCount, xnCount, xnCountChanged, 0)
//    double xnAngle, xnAngle, setXnAngle, xnAngleChanged, 0.0)
//    double xnShiftAngle, xnShiftAngle, setXnShiftAngle, xnShiftAngleChanged, 0.0)
//    double xnRotation, xnRotation, xnRotation, xnRotationChanged, 0.0)
//    double noiseCoeficient, noiseCoeficient, setNoiseCoeficient, noiseCoeficientChanged, 0.0)
//    double platina5AssistGain, platina5AssistGain, setPlatina5AssistGain, platina5AssistGainChanged, 0.0)
//    K_AUTO_PROPERTY(, soundSpeed, soundSpeed, setSoundSpeed, soundSpeedChanged, )
//    std::atomic<double> soundSpeed = 0;
//    std::atomic<double> impulseTime = 0;
//    std::atomic<double> noiseCoeficient;
protected:
    double sight = 0.0;
    int board = 0;
    double sightBrightness = 0.0;
//    double impulseTime = 0.0;
//    double distanceMode, distanceMode, setDistanceMode, distanceModeChanged, 0.0)
    double m_rightBoardStart = 0;
    double m_rightBoardEnd = 110;
    double m_leftBoardStart = 240;
    double m_leftBoardEnd = 350;
    QVector<double> lastSweep;
//    KVectorDiscretizer discretizer;

//    XnPressureConvertor *spatialProcessor = new XnPressureConvertor(this);

public:

    EPO1Painter(MGKAbstractMonitor * parent);

public:
    double rightBoardStart() const { return m_rightBoardStart; }
    void setRightBoardStart(double value) { m_rightBoardStart = value; }
    double rightBoardEnd() const { return m_rightBoardEnd; }
    void setRightBoardEnd(double value) { m_rightBoardEnd = value; }
    double leftBoardStart() const { return m_leftBoardStart; }
    void setLeftBoardStart(double value) { m_leftBoardStart = value; }
    double leftBoardEnd() const { return m_leftBoardEnd; }
    void setLeftBoardEnd(double value) { m_leftBoardEnd = value; }
};

#endif // EPO1PAINTER_H
