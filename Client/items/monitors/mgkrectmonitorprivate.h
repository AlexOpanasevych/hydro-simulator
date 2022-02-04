#ifndef MGKRECTMONITORPRIVATE_H
#define MGKRECTMONITORPRIVATE_H

#include <QQuickItem>
#include <items/mgkabstractmonitor.h>

class MGKRectMonitorPrivate : public MGKAbstractMonitor
{
    Q_OBJECT
    K_QML_TYPE(MGKRectMonitorPrivate)

public:
    explicit MGKRectMonitorPrivate(QQuickItem * parent = nullptr);
signals:


    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter);

public slots:
    void onSizeChanged();
    virtual void onModeChanged();
    void onMonitorWorkChanged();
};

#endif // MGKRECTMONITOR_H
