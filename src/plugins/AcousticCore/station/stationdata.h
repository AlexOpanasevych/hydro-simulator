#ifndef STATIONDATA_H
#define STATIONDATA_H

#include <QObject>
#include <base/physical_SI.h>
#include <base/sceneitem.h>
#include <QColor>

class StationData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(PhysicalItem* sceneItem READ sceneItem WRITE setSceneItem NOTIFY sceneItemChanged)
    Q_PROPERTY(double rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(double stationRotation READ stationRotation WRITE setStationRotation NOTIFY stationRotationChanged)
    Q_PROPERTY(double x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(double y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(int xnCount READ xnCount WRITE setXnCount NOTIFY xnCountChanged)
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY colorChanged)

public:
    explicit StationData(QObject *parent = nullptr);
    enum Mode{
        PLATINA = 0x01,
        BRONZE = 0x02
    };
    Q_ENUM(Mode)

    const QColor &color() const;
    void setColor(const QColor &newColor);

public slots:
    //! [property]
    double x() const;
    double y() const;
    Mode mode() const;
    int xnCount() const;
    double rotation() const;
    PhysicalItem* sceneItem() const;
    double stationRotation() const;

    void setX(double x);
    void setY(double y);
    void setMode(Mode mode);
    void setRotation(double angle);
    void setXnCount(int newXnCount);
    void setSceneItem(PhysicalItem* sceneItem);
    void setStationRotation(double stationRotation);

    //! [methods]
    void move(QPointF pos);

signals:
    void modeChanged(Mode mode);
    void sceneItemChanged(SceneItem* sceneItem);
    void rotationChanged(double angle);
    void xChanged(double x);
    void yChanged(double y);
    void stationRotationChanged(double stationRotation);
    void xnCountChanged();


    void colorChanged();

private:
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged)
    Mode m_mode = PLATINA;
    PhysicalItem* m_sceneItem = nullptr;
    double m_rotation = 0;
    double m_x = 0;
    double m_y = 0;
    double m_stationRotation = 0;
    int m_xnCount = 12;
    QColor m_color = "red";
};

#endif // STATIONDATA_H
