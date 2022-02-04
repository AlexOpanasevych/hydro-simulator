#ifndef MGKABSTRACTMONITOR_H
#define MGKABSTRACTMONITOR_H

#include <kmacro.h>
#include <QQuickPaintedItem>
#include <QImage>

class MonitorPainter;

struct SpatialChannel {
    std::vector<double> freqChannels;
};


//class MGKAbstractMonitorPainter : public QNanoQuickItemPainter {
//    QNanoColor backgroundColor;
//    QNanoColor color;
//    double brightness;
//    double cursorBrightness;
//    MonitorPainter * activePainter = nullptr;
//    bool firstTime = true;
//public:
//    MGKAbstractMonitorPainter();
//    void paint(QPainter *painter) override;
//    void synchronize(QNanoQuickItem *item) override;
//    QNanoColor getBackgroundColor() const;
//    QNanoColor getColor() const;
//    void setColor(const QNanoColor &value);
//    void setBackgroundColor(const QNanoColor &value);
//    MonitorPainter *getActivePainter() const;
//    void setActivePainter(MonitorPainter *value);
//};
//enum class Painter {RectPainter, TucPainter};
enum class SearchAngleMode {Search120 = 0, Search60};
enum class BoardMode {LEFT = 0, RIGHT};
enum class SearchScale {Scale1 = 0, Scale2, Scale3, Scale4};
enum class EpO1Mode {O1 = 0, O2, S1, S2, Shp};
enum class SearchMode {DPBP = 0, DP, BP};

Q_DECLARE_METATYPE(SearchAngleMode)
Q_DECLARE_METATYPE(BoardMode)
Q_DECLARE_METATYPE(SearchScale)
Q_DECLARE_METATYPE(EpO1Mode)
Q_DECLARE_METATYPE(SearchMode)

struct Cursor;

class MGKAbstractMonitor : public QQuickPaintedItem
{
    Q_OBJECT
    K_QML_INTERFACE(MGKAbstractMonitor)
    K_AUTO_PROPERTY(QColor, color, color, setColor, colorChanged, QColor("#ffff00"))
    K_AUTO_PROPERTY(QColor, backgroundColor, backgroundColor, setBackgroundColor, backgroundColorChanged, QColor("#073541"))
    K_AUTO_PROPERTY(double, afterShiningIntencity, afterShiningIntencity, setAfterShiningIntencity, afterShiningIntencityChanged, 1.0)
    K_AUTO_PROPERTY(QString, mode, mode, setMode, modeChanged, "o1-120-lb")
    K_AUTO_PROPERTY(int, sUpdateInterval, sUpdateInterval, setSUpdateInterval, sUpdateIntervalChanged, 20)
//    K_AUTO_PROPERTY(Painter, painter, painter, setPainter, painterChanged, Painter::RectPainter)
    K_PROTECTED_PROPERTY(Cursor * , screenCursor, screenCursor, setScreenCursor, screenCursorChanged, nullptr)
    K_AUTO_PROPERTY(double, maxTime, maxTime, setMaxTime, maxTimeChanged, 0)
    K_PROTECTED_PROPERTY(MonitorPainter *, activePainter, activePainter, setActivePainter, activePainterChanged, nullptr)
    K_AUTO_PROPERTY(double, distance, distance, setDistance, distanceChanged, 0.0)
    K_AUTO_PROPERTY(double, maxDistance, maxDistance, setMaxDistance, maxDistanceChanged, 4000.0)
    K_AUTO_PROPERTY(double, peleng, peleng, setPeleng, pelengChanged, 0.0)
    K_READONLY_PROPERTY(double, maxPeleng, maxPeleng, setMaxPeleng, maxPelengChanged, 120)
    K_AUTO_PROPERTY(double, brightness, brightness, setBrightness, brightnessChanged, 0)
    K_AUTO_PROPERTY(double, amplifier, amplifier, setAmplifier, amplifierChanged, 0)
    K_AUTO_PROPERTY(SearchAngleMode, searchAngleMode, searchAngleMode, setSearchAngleMode, searchAngleModeChanged, SearchAngleMode::Search120)
    K_AUTO_PROPERTY(BoardMode, boardMode, boardMode, setBoardMode, boardModeChanged, BoardMode::LEFT)
    K_AUTO_PROPERTY(int, soundSpeed, soundSpeed, setSoundSpeed, soundSpeedChanged, 1481)
    K_AUTO_PROPERTY(int, impulseTime, impulseTime, setImpulseTime, impulseTimeChanged, 200)
    K_AUTO_PROPERTY(QString, streamName, streamName, setStreamName, streamNameChanged, "")
    K_AUTO_PROPERTY(SearchScale, searchScale, searchScale, setSearchScale, searchScaleChanged, SearchScale::Scale1)
    K_AUTO_PROPERTY(EpO1Mode, epMode, epMode, setEpMode, epModeChanged, EpO1Mode::O1)
    K_AUTO_PROPERTY(SearchMode, searchMode, searchMode, setSearchMode, searchModeChanged, SearchMode::DPBP)
    K_AUTO_PROPERTY(bool, workStarted, workStarted, setWorkStarted, workStartedChanged, false)
    K_AUTO_PROPERTY(bool, monitorWork, monitorWork, setMonitorWork, monitorWorkChanged, true)

    QTimer * surfaceUpdateTimer = nullptr;
public:
    MGKAbstractMonitor(QQuickItem * parent = nullptr);
    ~MGKAbstractMonitor();

    QImage drawImage;
    QImage mask;
public slots:
    void updateSurface();
    void updateAfterShining();

//    void onSearchScaleChanged(SearchScale);

    void onSUpdateIntervalChanged(int);

//    void onMonitorWorkChanged()

    // QQuickPaintedItem interface
public:
    void paint(QPainter *painter);
    QRegion getMaskAsRegion();
signals:
    void sizeChanged();

public slots:
    void onSizeChanged();
};

template<class T>
class Prototype {
public:
    virtual T clone() = 0;
};

struct Cursor : public QObject, public Prototype<Cursor*> {
    Q_OBJECT
    K_QML_TYPE(Cursor)
    K_READONLY_PROPERTY(double, cursorBrightness, cursorBrightness, setCursorBrightness, cursorBrightnessChanged, 1.0)
    K_READONLY_PROPERTY(double, size, size, setSize, sizeChanged, 2.0)
    K_AUTO_PROPERTY(MGKAbstractMonitor*, monitor, monitor, setMonitor, monitorChanged, nullptr)
    K_READONLY_PROPERTY(int, x, x, setX, xChanged, 40)
    K_READONLY_PROPERTY(int, y, y, setY, yChanged, 40)
//    K_AUTO_PROPERTY(MonitorPainter *, painter, painter, setPainter, painterChanged, nullptr)
public:
    Cursor(MGKAbstractMonitor * parent = nullptr) : QObject(parent), m_monitor(parent) {}
public slots:
    virtual void changePosition(const QVariantMap & data);
    virtual void paint(uint32_t *pixels);
    Cursor * clone() override;

};

struct LineCircleCursor : public Cursor {
private:
    Q_OBJECT
    K_AUTO_PROPERTY(int, angle, angle, setAngle, angleChanged, 0)
    K_AUTO_PROPERTY(int, length, length, setLength, lengthChanged, 0)
    K_AUTO_PROPERTY(int, centerMargin, centerMargin, setCenterMargin, centerMarginChanged, 0)
public:
    LineCircleCursor(MGKAbstractMonitor * parent = nullptr) : Cursor(parent) {}
public slots:
    void changePosition(const QVariantMap & data) override;
    void paint(uint32_t *pixels) override;
};


#endif // MGKABSTRACTMONITOR_H
