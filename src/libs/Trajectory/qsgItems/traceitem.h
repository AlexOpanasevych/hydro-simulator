#ifndef TRACEITEM_H
#define TRACEITEM_H

#include <QQuickItem>
#include <libs/klibcorelite/kmacro.h>

class TraceItem : public QQuickItem
{
    Q_OBJECT
    K_AUTO_PROPERTY(int, bufferLength, bufferLength, setBufferLength, bufferLengthChanged, 300)
public:
    TraceItem();
public slots:
    void updateCoords(QPointF coords, bool clear = false);
signals:
    // QQuickItem interface
protected:
    virtual QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;
private:
    QVector<QPointF> points;
    static inline const int TraceItemQMLRegistration = qmlRegisterType<TraceItem>("Trajectory", 1, 0, "TraceItem");
};

#endif // TRACEITEM_H
