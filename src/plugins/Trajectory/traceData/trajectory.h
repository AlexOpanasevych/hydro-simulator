#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <QObject>
#include <libs/klibcorelite/kmacro.h>
#include <utility/kobservablelist.h>
#include <QPointF>
#include <QColor>
#include <math.h>
#include <complex>
#include <libs/PluginsBase/base/sceneitem.h>
#include <trajectory_global.h>
#define Y_SWAP -1

struct TRAJECTORY_EXPORT TrajectorySegment : public QObject
{
    Q_OBJECT
    K_AUTO_PROPERTY(QString, type, type, setType, typeChanged, "")
    K_AUTO_PROPERTY(double, depth, depth, setDepth, depthChanged, 0)
    K_AUTO_PROPERTY(QColor, color, color, setColor, colorChanged, "black")
    K_AUTO_PROPERTY(QColor, selectedColor, selectedColor, setSelectedColor, setSelectedColorChanged, "red")
    K_AUTO_PROPERTY(bool, selected, selected, setSelected, selectedChanged, false)
    K_AUTO_PROPERTY(double, velocity, velocity, setVelocity, velocityChanged, 0)
    K_AUTO_PROPERTY(QVector<QPointF>, points, points, setPoints, pointsChanged, QVector<QPointF>())
    friend class Trajectory;
public:
    TrajectorySegment(QObject *parent = nullptr) : QObject(parent) {};
    virtual void build(QPointF p1, QPointF p2) = 0;
};

struct TRAJECTORY_EXPORT LineSegment : public TrajectorySegment
{
    Q_OBJECT
    //QML_TYPE(LineSegment)

    // TrajectorySegment interface
    public:
        LineSegment(QObject *parent = nullptr) : TrajectorySegment(parent) { setType("Line"); };

    // TrajectorySegment interface
public:
    virtual void build(QPointF p1, QPointF p2) override { setPoints({p1, p2}); };
};

struct TRAJECTORY_EXPORT CircleSegment : public TrajectorySegment
{
    Q_OBJECT
    //QML_TYPE(CircleSegment)
    K_READONLY_PROPERTY(QPointF, c, c, setC, cChanged, QPointF(0,0))
    double R;
    double fromAngle = 0;
    double toAngle = 360;
    int pointsCount = 20;

    // TrajectorySegment interface
    public:
        CircleSegment(QObject *parent = nullptr) : TrajectorySegment(parent) { setType("Circle"); };
        void setup(QPointF p, double r, double K, double angle);
        virtual void build(QPointF p1, QPointF p2) override;
        QPointF getEndPoint();
        double getEndCourse();
};

struct TRAJECTORY_EXPORT FireSegment : public TrajectorySegment
{
    Q_OBJECT
    //QML_TYPE(FireSegment)
    K_READONLY_PROPERTY(double, rotation, rottaion, setRotation, rotationChanged, 0)
    public:
        FireSegment(QObject *parent = nullptr) : TrajectorySegment(parent) { setType("Torpedo"); };

    // TrajectorySegment interface
public:
    virtual void build(QPointF p1, QPointF p2) override { setPoints({p1, p2}); };
};

class Cmd;
class Param;
class TRAJECTORY_EXPORT Trajectory : public QObject
{
    Q_OBJECT
    //QML_TYPE(Trajectory)
    K_READONLY_PROPERTY(SceneItem *, item, item, setItem, itemChanged, nullptr)
    K_AUTO_PROPERTY(double, x, x, setX, xChanged, 0)
    K_AUTO_PROPERTY(double, y, y, setY, yChanged, 0)
    K_AUTO_PROPERTY(double, opacity, opacity, setOpacity, opacityChanged, 1)
    K_AUTO_PROPERTY(QString, properties, properties, setProperties, propertiesChanged, "")
    K_AUTO_PROPERTY(QVector<TrajectorySegment *>, vertexes, vertexes, setVertexes, vertexesChanged, QVector<TrajectorySegment *>())
public:
    explicit Trajectory(QObject *parent = nullptr);
public slots:
    void updateProperties();
    void selectTrajectory(int place);
    void move(QPointF point);

private:
    KObservableList<TrajectorySegment> m_trajectories;

    QVector<QPointF> points;
    QVector<int> cmdPos;

    double getParamValue(QList<Cmd *> *list, Cmd *cmd, QString param);
};


#endif // TRAJECTORY_H
