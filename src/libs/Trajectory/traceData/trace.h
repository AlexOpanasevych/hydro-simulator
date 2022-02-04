#ifndef TRACE_H
#define TRACE_H

#include <QObject>
#include <libs/klibcorelite/kmacro.h>
#include <libs/klibcorelite/utility/kobservablelist.h>
#include <QPointF>
#include <traceData/segmentdata.h>
#include <trajectory_global.h>

class TRAJECTORY_EXPORT Trace : public QObject
{
    Q_OBJECT
    //QML_TYPE(Trace)
    K_READONLY_PROPERTY(KObservableModel *, segments, segments, setSegments, segmentsChanged, nullptr)
    K_AUTO_PROPERTY(int, bufferSize, bufferSize, setBufferSize, bufferSizeChanged, 300)
public:
    explicit Trace(QObject *parent = nullptr);
    enum TraceMode{
        START_SEGMENT = 0,
        EXPAND_SEGMENT,
        END_SEGMENT,
        DEFAULT
    };
signals:
    void coordsUpdated(QPointF pos);
public slots:
    void deserialize(QVariant);
    void setCoords(QVector<QPointF> point, TraceMode mode);
    void updateCoords(QPointF point, TraceMode mode = Trace::DEFAULT);
    void updateCoords(double width, double height, TraceMode mode);
    void disselectAllItems();

private:
    K_READONLY_PROPERTY(TraceMode, traceMode, traceMode, setTraceItem, traceItemChanged, Trace::DEFAULT)
    KObservableList<SegmentData> m_segmentsList;
    QVector<QPointF> m_points;

    void addToBuffer(QPointF point);

};
#endif // TRACE_H
