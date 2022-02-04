#ifndef SEGMENTITEM_H
#define SEGMENTITEM_H

#include <QQuickItem>
#include <libs/klibcorelite/kmacro.h>

class SegmentItem : public QQuickItem
{
    Q_OBJECT
    K_AUTO_PROPERTY(double, x, x, setX, xChanged, 0)
    K_AUTO_PROPERTY(double, y, y, setY, yChanged, 0)
    K_AUTO_PROPERTY(double, width, width, setWidth, widthChanged, 0)
    K_AUTO_PROPERTY(double, height, height, setHeight, heightChanged, 0)
    K_AUTO_PROPERTY(QString, color, color, setColor, colorChanged, "black")
    K_AUTO_PROPERTY(double, lineWidth, lineWidth, setLineWidth, lineWidthChanged, 1)
    //symmetric line (just simple trace item coords)
    K_AUTO_PROPERTY(QVector<QPointF>, points, points, setPoints, pointsChanged, QVector<QPointF>())
    K_READONLY_PROPERTY(QPointF, emittingPos, emittingPos, setEmittingPos, emittingPosChanged, QPointF(0,0))
public:
    SegmentItem();

protected:
    virtual QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;
private:
    static inline const int SegmentItemQMLRegistration = qmlRegisterType<SegmentItem>("Trajectory", 1, 0, "SegmentItem");
};

#endif // SEGMENTITEM_H
