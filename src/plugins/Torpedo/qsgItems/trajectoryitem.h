#ifndef TRAJECTORYITEM_H
#define TRAJECTORYITEM_H

#include <QQuickItem>

#include <sceneItems/trace/trajectory.h>

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
};

#endif // TRAJECTORYITEM_H
