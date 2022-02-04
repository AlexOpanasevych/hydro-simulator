#ifndef CUSTOMITEM_H
#define CUSTOMITEM_H

#include <qnanoquickitem.h>
#include <qnanoquickitempainter.h>
#include <kmacro.h>
#include <streamconnector.h>
#include <QImage>
#include <QTimer>
#include <QQueue>

//#define QNANO_USE_RENDERNODE

class CustomItemPainter : public QNanoQuickItemPainter
{
//    Q_OBJECT
    QNanoImage backgroundImage;
    QNanoImage linealImage;
    QNanoImage frameImage;

    QNanoImage backgroundOrigin;

    bool needToRedrawImage = false;
    bool needToRedrawPoints = false;

    QVector<QPointF> points;

    int margin = 12;
public:
    CustomItemPainter()
    {
    }

    void paint(QNanoPainter *p) override;

    void synchronize(QNanoQuickItem *item) override;
};

class CustomItem : public QNanoQuickItem
{
    friend class CustomItemPainter;
    Q_OBJECT
    K_QML_TYPE(CustomItem)

    QTimer * updateTimer = new QTimer(this);

    K_AUTO_PROPERTY(QString, backgroundImageSource, backgroundImageSource, setBackgroundImageSource, backgroundImageSourceChanged, "")
    K_AUTO_PROPERTY(QString, linealImageSource, linealImageSource, setLinealImageSource, linealImageSourceChanged, "")
    K_AUTO_PROPERTY(QString, frameImageSource, frameImageSource, setFrameImageSource, frameImageSourceChanged, "")

    K_AUTO_PROPERTY(double, maxDistance, maxDistance, setMaxDistance, maxDistanceChanged, 100.0)
    K_AUTO_PROPERTY(double, distance, distance, setDistance, distanceChanged, 0.0)
    K_AUTO_PROPERTY(double, currentDistance, currentDistance, setCurrentDistance, currentDistanceChanged, 0.0)
    K_AUTO_PROPERTY(QString, epStationState, epStationState, setEpStationState, epStationStateChanged, "")
    K_AUTO_PROPERTY(double, emissionCycleTime, emissionCycleTime, setEmissionCycleTime, emissionCycleTimeChanged, 0.0)


    QElapsedTimer elapsedTimer;

    double elapsedFromLastDrawing = 0;

    double startPosition = 88;

    double backgroundOffset = 0;
    double backgroundPosition = -startPosition;
    double backgroundGlobalPosition = -startPosition;


    int lastPointX = 0, lastPointY = 0;


public slots:
//    void updateRecorder();
signals:
public:
    CustomItem(QQuickItem * parent = nullptr);
    QNanoQuickItemPainter * createItemPainter() const override;
private:
    listener m_handler1;
    listener platinaWaveRecieved;
    QQueue<QVector<QPointF>> sweepQueue;
};

#endif // CUSTOMITEM_H
