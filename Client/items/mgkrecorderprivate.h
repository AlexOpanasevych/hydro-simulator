#ifndef MGKRECORDERPRIVATE_H
#define MGKRECORDERPRIVATE_H

#include <QQuickItem>
#include <kmacro.h>
#include <QImage>
#include <QElapsedTimer>
#include <streamconnector.h>
#include <QQueue>
#include <QTimer>

//#include <utility/xnpressureconvertor.h>

class MGKRecorderPrivate : public QQuickItem
{
    Q_OBJECT
    QImage backgroundImage;
//    QImage background;
    QImage linealImage;
    QImage frameImage;

    QImage backgroundOrigin;
    QImage backgroundMirrored;

    QTimer * updateTimer = new QTimer(this);

    int margin = 12;

    K_QML_TYPE(MGKRecorderPrivate)



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



public:
    MGKRecorderPrivate(QQuickItem * parent = nullptr);

    QSGNode * updatePaintNode(QSGNode *, UpdatePaintNodeData *) override;
public slots:
    void updateRecorder();
signals:
private:
    listener m_handler1;
    listener platinaWaveRecieved;
    QQueue<QVector<double>> sweepQueue;
    inline void setupProcessor();

//    QTimer * updateTimer = new QTimer(this);
};

#endif // MGKRECORDER_H
