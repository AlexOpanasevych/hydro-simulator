#ifndef SIMDATAMANAGER_H
#define SIMDATAMANAGER_H

#include <QObject>
#include <kmacro.h>

class SimDataManager : public QObject
{
    K_THREADSAFE_SINGLETON(SimDataManager)
    K_QML_SINGLETON(SimDataManager)
    Q_OBJECT

    K_AUTO_PROPERTY(float, courseAngle, courseAngle,setCourseAngle,courseAngleChanged, 0)
    K_AUTO_PROPERTY(int  , board      , board      ,setBoard      ,      boardChanged, 0)
    K_AUTO_PROPERTY(float, pelengAngle, pelengAngle,setPelengAngle,pelengAngleChanged, 0)
    K_AUTO_PROPERTY(float, speed      , speed      ,setSpeed      ,      speedChanged, 0)
    K_AUTO_PROPERTY(float, distance   , distance   ,setDistance   ,   distanceChanged, 0)

    explicit SimDataManager(QObject *parent = nullptr);
public:
    void fromMap(const QVariantMap & data);
//    float getDistance() const;
//    void setDistance(float distance);

//    float getSpeed() const;
//    void setSpeed(float speed);

//    float getPelengAngle() const;
//    void setPelengAngle(float pelengAngle);

//    int getBoard() const;
//    void setBoard(int board);

//    float getCourseAngle() const;
//    void setCourseAngle(float courseAngle);

signals:
};

#endif // SIMDATAMANAGER_H
