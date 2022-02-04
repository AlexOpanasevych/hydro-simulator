#ifndef TORPEDO_H
#define TORPEDO_H

#include <QObject>
#include <libs/klibcorelite/kmacro.h>
#include <traceData/trace.h>
#include <libs/PluginsBase/base/physical_SI.h>

enum Board {LEFT, RIGHT};

class Torpedo : public PhysicalItem
{
    Q_OBJECT
    K_CONST_PROPERTY(Trace *, trace, new Trace(this))
    K_AUTO_PROPERTY(QVariantList, stages, stages, setStages, stagesChanged, QVariantList())
    K_AUTO_PROPERTY(int, firstRa, firstRa, setFirstRa, firstRaChanged, 0)
    K_AUTO_PROPERTY(int, secondRa, secondRa, setSecondRa, secondRaChanged, 0)
    K_AUTO_PROPERTY(int, torpedoDivergency, torpedoDivergency, setTorpedoDivergency, torpedoDivergencyChanged, 0)
    K_AUTO_PROPERTY(double, firstRotateDistance, firstRotateDistance, setFirstRotateDistance, firstRotateDistanceChanged, 0)
    K_AUTO_PROPERTY(double, courseAngle, courseAngle, setCourseAngle, courseAngleChanged, 0)
    K_AUTO_PROPERTY(double, distanceToGoal, distanceToGoal, setDistanceToGoal, distanceToGoalChanged, 0)
    K_AUTO_PROPERTY(double, currentDistance, currentDistance, setCurrentDistance, currentDistanceChanged, 0)
    K_AUTO_PROPERTY(double, currentTime, currentTime, setCurrentTime, currentTimeChanged, 0)
    K_AUTO_PROPERTY(bool, goalFound, goalFound, setGoalFound, goalFoundChanged, 0)
    Q_ENUM(Board)
    K_AUTO_PROPERTY(Board, goalBoard, goalBoard, setGoalBoard, goalBoardChanged, Board::LEFT)
    K_AUTO_PROPERTY(int, boardAngle, boardAngle, setBoardAngle, boardAngleChanged, 10)
public:
    Torpedo(QObject *parent = nullptr);
    void proceed(SampleDataGetter &getter) override;

    virtual QVariantMap serialize() override;
    virtual void deserialize(QVariantMap map) override;


    // SceneItem interface
public:
    void init(EngineInterface * ei) override;
};


#endif // TORPEDO_H
