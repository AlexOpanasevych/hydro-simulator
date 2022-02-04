#ifndef STATIONITEMQSG_H
#define STATIONITEMQSG_H

#include <QQuickItem>
#include <station/xnleaf.h>

class StationItemQSG : public QQuickItem
{
    Q_OBJECT
    Q_PROPERTY(double startAngle READ startAngle WRITE setStartAngle NOTIFY startAngleChanged)
    Q_PROPERTY(double arcAngle READ arcAngle WRITE setArcAngle NOTIFY arcAngleChanged)
    Q_PROPERTY(int xnCount READ xnCount WRITE setXnCount NOTIFY xnCountChanged)
    Q_PROPERTY(int mode READ mode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)
public:
    StationItemQSG(QQuickItem *parent = nullptr);



public slots:
    int mode() const;
    int xnCount() const;
    double arcAngle() const;
    double startAngle() const;
    QColor color() const;

    void setMode(int mode);
    void setXnCount(int xnCount);
    void setArcAngle(double arcAngle);
    void setStartAngle(double startAngle);
    void setColor(QColor newColor);

signals:
    void xnCountChanged(int xnCount);
    void arcAngleChanged(double arcAngle);
    void startAngleChanged(double startAngle);
    void modeChanged(int mode);
    void colorChanged();


private:
    int m_xnCount = 36;
    double m_arcAngle = 10;
    double m_startAngle = -5;
    int m_mode = 0;

    QList<XNLeaf*> m_xnList;
    // QQuickItem interface
    QColor m_color = "red";

protected:
    virtual QSGNode *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *) override;
};


#endif // STATIONITEMQSG_H
