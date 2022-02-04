#ifndef TRAJECTORYITEM_H
#define TRAJECTORYITEM_H

#include <QQuickItem>

#include <traceData/trajectory.h>

class QSGGeometryNode;
class TrajectoryItem : public QQuickItem
{
    Q_OBJECT
    K_AUTO_PROPERTY(Trajectory *, trajectory, trajectory, setTrajectory, trajectoryChanged, nullptr)
public:
    TrajectoryItem(QQuickItem *parent = nullptr);

signals:

    // QQuickItem interface
protected:
    virtual QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;

private slots:
    void buildLine(QSGGeometryNode * n, LineSegment *segment);
    void buildFire(QSGGeometryNode * n, FireSegment *segment);
    void buildCircle(QSGGeometryNode * n, CircleSegment *segment);
    void clearChilds(QSGNode *n);

private:
    static inline const int TrajectoryItemQMLRegistration = qmlRegisterType<TrajectoryItem>("Trajectory", 1, 0, "TrajectoryItem");

};

#endif // TRAJECTORYITEM_H
