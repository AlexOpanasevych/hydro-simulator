#ifndef DOPLERPAINTER_H
#define DOPLERPAINTER_H

#include "sweeppainter.h"
#include <kmacro.h>
#include <utility/kvectordiscretizer.h>
#include <streamconnector.h>

class DoplerPainter : public SweepPainter
{
//    K_AUTO_PROPERTY(double, gain, gain, setGain, gainChanged, 0.0                             ) // unused
//    K_AUTO_PROPERTY(double, sight, sight, setSight, sightChanged, 0.0                         ) // unused
//    K_AUTO_PROPERTY(double, distanceHelm, distanceHelm, setDistanceHelm, distanceHelmChanged, 0.0) // unused
//    K_AUTO_PROPERTY(int   , board, board, setBoard, boardChanged, 0                           ) // unused
    K_AUTO_PROPERTY(double, rightSightBrightness, rightSightBrightness, setRightSightBrightness, rightSightBrightnessChanged, 1.0)
//    K_AUTO_PROPERTY(double, impulseTime, impulseTime, setImpulseTime, impulseTimeChanged, 0.0) // unused
//    K_AUTO_PROPERTY(double, soundSpeed, soundSpeed, setSoundSpeed, soundSpeedChanged, 0.0     ) // unused
//    K_AUTO_PROPERTY(int   , distanceMode, distanceMode, setDistanceMode, distanceModeChanged, 0) // unused
    K_AUTO_PROPERTY(int , doplerChannels, doplerChannels, setDoplerChannels, doplerChannelsChanged, 0)

    listener m_handler = nullptr;
public:
    DoplerPainter(MGKAbstractMonitor * parent);
//    void paint(QPainter * painter) override;
//    void onSpChannelCntChanged(int spChannelCnt);
};

#endif // DOPLERPAINTER_H
