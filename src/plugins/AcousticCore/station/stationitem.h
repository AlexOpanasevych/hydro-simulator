#ifndef STATIONITEM_H
#define STATIONITEM_H

#include <QQuickItem>
#include <QQuickPaintedItem>

class StationItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int xnCount READ xnCount WRITE setXnCount NOTIFY xnCountChanged)
    Q_PROPERTY(double angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(double shiftAngle READ shiftAngle WRITE setShiftAngle NOTIFY shiftAngleChanged)
public:
    StationItem(QQuickItem *parent = nullptr);
public:
    virtual void paint(QPainter *painter) override;

public slots:
    int xnCount() const;
    double angle() const;
    double shiftAngle() const;

    void setXnCount(int xnCount);
    void setAngle(double angle);
    void setShiftAngle(double shiftAngle);

signals:
    void xnCountChanged(int xnCount);
    void angleChanged(double angle);
    void shiftAngleChanged(double shiftAngle);

private:
    int m_xnCount = 12;
    double m_angle = 5;
    double m_shiftAngle = 10;

};

#endif // STATIONITEM_H
