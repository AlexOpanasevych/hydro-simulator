#include "mgkabstractmonitor.h"
#include <QTimer>
#include <items/painters/doplerpainter.h>

#include <QTimer>
#include <items/painters/csnrpainter.h>
#include <items/painters/epo160painter.h>
#include <items/painters/epo1painter.h>
#include <items/painters/tucpainter.h>
#include <math/kmath.h>
#include <math.h>
#include <QPainter>
#include <QBitmap>


//#define bounded(value, bottom, top) std::min(std::max(bottom, value), top)

QRegion MGKAbstractMonitor::getMaskAsRegion() {
    return QRegion(QBitmap::fromImage(mask.createMaskFromColor(0)));
}

MGKAbstractMonitor::MGKAbstractMonitor(QQuickItem *parent) : QQuickPaintedItem(parent)
{

    qRegisterMetaType<SearchScale>();
    qRegisterMetaType<SearchAngleMode>();
    qRegisterMetaType<BoardMode>();
    qRegisterMetaType<EpO1Mode>();
    qRegisterMetaType<SearchMode>();

    auto calcMaxDistance = [this](){
        switch(m_epMode) {
        case EpO1Mode::O1:
            switch (m_searchMode) {
            case SearchMode::DPBP:
                [[fallthrough]];
            case SearchMode::DP:
                switch (m_searchScale) {
                case SearchScale::Scale1:
                    setMaxDistance(8000); // 2 * scale distance
                    break;
                case SearchScale::Scale3:
                    setMaxDistance(32000);
                    break;
                case SearchScale::Scale4:
                    setMaxDistance(64000);
                    break;
                case SearchScale::Scale2:
                    setMaxDistance(16000);
                    break;
    //                [[fallthrough]];
    //            case SearchScale::ZeroScale:
    //                setMaxDistance(16000);
    //                break;
                }
                break;
            case SearchMode::BP:
                setMaxDistance(16000);
                break;
            }
            break;
        case EpO1Mode::O2:
            setMaxDistance(16000);
            break;
        case EpO1Mode::S1:
            [[fallthrough]];
        case EpO1Mode::S2:
            setMaxDistance(16000);
            break;
        case EpO1Mode::Shp:
            break;
        }

    };
    connect(this, &MGKAbstractMonitor::searchScaleChanged, this, calcMaxDistance);
    connect(this, &MGKAbstractMonitor::searchModeChanged, this, calcMaxDistance);
    connect(this, &MGKAbstractMonitor::epModeChanged, this, calcMaxDistance);
//    connect(this, &MGKAbstractMonitor::ModeChanged, this, calcMaxDistance);

    auto calcMaxTime = [this](){
        setMaxTime(maxDistance() / soundSpeed());
    };


    calcMaxDistance();
    calcMaxTime();

    connect(this, &MGKAbstractMonitor::maxDistanceChanged, this, calcMaxTime);
    connect(this, &MGKAbstractMonitor::soundSpeedChanged, this, calcMaxTime);

    setScreenCursor(new Cursor(this));
    screenCursor()->setMonitor(this);

    surfaceUpdateTimer = new QTimer(this);
    connect(surfaceUpdateTimer, &QTimer::timeout, this, &MGKAbstractMonitor::updateSurface);
    surfaceUpdateTimer->start(sUpdateInterval());

    connect(this, &MGKAbstractMonitor::brightnessChanged, this, &MGKAbstractMonitor::updateSurface);
    connect(this, &MGKAbstractMonitor::colorChanged, this, [this](){
        qDebug() << "color changed" << color();
    });

    connect(this,
            &MGKAbstractMonitor::searchAngleModeChanged,
            this,
            [this](SearchAngleMode mode){setMaxPeleng(mode == SearchAngleMode::Search120 ? 120 : 60);});


    connect(this, &QQuickItem::heightChanged, this, &MGKAbstractMonitor::sizeChanged);
    connect(this, &QQuickItem::widthChanged, this, &MGKAbstractMonitor::sizeChanged);
    connect(this, &MGKAbstractMonitor::sizeChanged, this, &MGKAbstractMonitor::onSizeChanged);


}

void MGKAbstractMonitor::onSizeChanged()
{
    int w = static_cast<int>(width());
    int h = static_cast<int>(height());
    drawImage = QImage(w, h, QImage::Format_ARGB32);
    drawImage.fill(0);
}

MGKAbstractMonitor::~MGKAbstractMonitor()
{

}

//QNanoQuickItemPainter *MGKAbstractMonitor::createItemPainter() const
//{
//    return new MGKAbstractMonitorPainter;
//}

void MGKAbstractMonitor::updateSurface()
{
    updateAfterShining();
    update();
}

//void MGKAbstractMonitor::onModeChanged(QString mode)
//{
//    Q_UNUSED(mode)

//    if(mode == "o1-120-lb") {
//        activePainter()->deleteLater();
//        setActivePainter(new EPO1Painter(this));
//        setEpMode(EpO1Mode::O1);
//        setSearchScale(SearchScale::Scale2);
//        setBoardMode(BoardMode::LEFT);
//        setSearchAngleMode(SearchAngleMode::Search120);
//    }
//    if(mode == "o1-120-pb") {
//        activePainter()->deleteLater();
//        setActivePainter(new EPO1Painter(this));
//        setEpMode(EpO1Mode::O1);
//        setSearchScale(SearchScale::Scale2);
//        setBoardMode(BoardMode::RIGHT);
//        setSearchAngleMode(SearchAngleMode::Search120);
//    }
//    if(mode == "o1-60-lb") {
//        activePainter()->deleteLater();
//        setActivePainter(new Epo160Painter(this));
//        setEpMode(EpO1Mode::O1);
//        setSearchScale(SearchScale::Scale1);
//        setBoardMode(BoardMode::LEFT);
//        setSearchAngleMode(SearchAngleMode::Search60);
//    }
//    if(mode == "o1-60-pb") {
//        activePainter()->deleteLater();
//        setActivePainter(new Epo160Painter(this));
//        setEpMode(EpO1Mode::O1);
//        setSearchScale(SearchScale::Scale1);
//        setBoardMode(BoardMode::RIGHT);
//        setSearchAngleMode(SearchAngleMode::Search60);
//    }
//    if(mode == "o2") {
//        activePainter()->deleteLater();
//        setActivePainter(new DoplerPainter(this));
//        setEpMode(EpO1Mode::O1);
//        setSearchScale(SearchScale::ZeroScale);
//        setBoardMode(BoardMode::RIGHT);
//        setSearchAngleMode(SearchAngleMode::Search120);
//    }

//    if(mode == "ep-01-bp") {
//        activePainter()->deleteLater();
//        setActivePainter(new EPO1Painter(this));
//        setEpMode(EpO1Mode::O1);
//        setSearchScale(SearchScale::ZeroScale);
//        setBoardMode(BoardMode::RIGHT);
//        setSearchAngleMode(SearchAngleMode::Search120);
//    }

//    if(mode == "asc") {
//        activePainter()->deleteLater();
//        setActivePainter(new CSNRPainter(this));
//        screenCursor()->deleteLater();
//        auto lineCursor = new LineCircleCursor;
//        lineCursor->setMonitor(this);
//        setScreenCursor(lineCursor);
//    }
//    if(mode == "s1") {
//        activePainter()->deleteLater();
//        auto painter = new SweepPainter(this);
//        painter->setGroupCount(6);
//        painter->setTracksInGroup(6);
//        setActivePainter(painter);
//        setSearchScale(SearchScale::ZeroScale);
//        setBoardMode(BoardMode::RIGHT);
//        setSearchAngleMode(SearchAngleMode::Search120);
//        setEpMode(EpO1Mode::S1);
////        screenCursor()->deleteLater();
////        auto lineCursor = new LineCircleCursor;
////        lineCursor->setMonitor(this);
////        setScreenCursor(lineCursor);
    //    }
//}

void MGKAbstractMonitor::onSUpdateIntervalChanged(int newUpdateInterval)
{
    surfaceUpdateTimer->setInterval(newUpdateInterval);
}

void MGKAbstractMonitor::paint(QPainter *painter)
{
    painter->drawImage(0, 0, drawImage);
    if(activePainter())
        activePainter()->paint(painter);
}

//QNanoColor MGKAbstractMonitorPainter::getBackgroundColor() const
//{
//    return backgroundColor;
//}

//QNanoColor MGKAbstractMonitorPainter::getColor() const
//{
//    return color;
//}

//void MGKAbstractMonitorPainter::setColor(const QNanoColor &value)
//{
//    color = value;
//}

//void MGKAbstractMonitorPainter::setBackgroundColor(const QNanoColor &value)
//{
//    backgroundColor = value;
//}

//MonitorPainter *MGKAbstractMonitorPainter::getActivePainter() const
//{
//    return activePainter;
//}

//void MGKAbstractMonitorPainter::setActivePainter(MonitorPainter *value)
//{
//    activePainter = value;
//}

//MGKAbstractMonitorPainter::MGKAbstractMonitorPainter()
//{

//}

//void MGKAbstractMonitorPainter::paint(QPainter *painter)
//{
//    if(activePainter) {
//        activePainter->paint(painter);
//    }
//}

//void MGKAbstractMonitorPainter::synchronize(QNanoQuickItem *item)
//{
//    auto monitorItem = static_cast<MGKAbstractMonitor*>(item);
//    if(activePainter != monitorItem->activePainter())
//        activePainter = monitorItem->activePainter();
//}

void LineCircleCursor::changePosition(const QVariantMap &data)
{
    Cursor::changePosition(data);
    auto angle = data["cursorAngle"].toString();
    if(angle == "increment") {
        this->m_angle++;
        emit angleChanged(m_angle);
    }
    if(angle == "decrement") {
        this->m_angle--;
        emit angleChanged(m_angle);
    }
}

void LineCircleCursor::paint(uint32_t *pixels)
{
    const double angle = (this->m_angle - 90) * M_PI / 180;
    const double sin = std::sin(angle);
    const double cos = std::cos(angle);
    auto lineBeginX = monitor()->width() / 2 + cos * m_centerMargin;
    auto lineBeginY = monitor()->height() / 2 + sin * m_centerMargin;
    auto cursorEndX = lineBeginX - monitor()->width() / 4 * cos;
    auto cursorEndY = lineBeginY - monitor()->height() / 4 * sin;
    monitor()->activePainter()->drawPrettyLine(pixels, lineBeginX, lineBeginY, cursorEndX, cursorEndY, cursorBrightness(), 1, monitor()->color(), monitor()->backgroundColor());

}

void Cursor::changePosition(const QVariantMap &data)
{
    bool ok;
    if(data["distance"].canConvert<QVariantMap>()) {
        auto distance = data["distance"].toMap();

        auto fullTurn = distance["fullTurn"].toInt();
        auto value = distance["value"].toDouble();

        monitor()->setDistance(KMath::dmap(KMath::limitValue(
                                               360 * fullTurn + value,
                                               0,
                                               360. * 7),
                                           0,
                                           360. * 7,
                                           0,
                                           monitor()->maxDistance()
                                           ));
        setY(KMath::dmap(
                 monitor()->distance(),
                 0, // IN MIN
                 monitor()->maxDistance(), // IN MAX
                 0,
                 monitor()->height())
             );
    }
    if(data["peleng"].canConvert<QVariantMap>()) {
        auto peleng = data["peleng"].toMap();
        auto fullTurn = peleng["fullTurn"].toInt();
        auto value = peleng["value"].toDouble();
        monitor()->setPeleng(KMath::dmap(KMath::limitValue(
                                             360 * fullTurn + value,
                                             0,
                                             360. * 7),
                                         0,
                                         360. * 7,
                                         0,
                                         monitor()->maxPeleng()
                                         ));
        setX(KMath::dmap(
                 monitor()->peleng(),
                 0, // IN MIN
                 monitor()->maxPeleng(), // IN MAX
                 0,
                 monitor()->width()));
    }
    auto brightness = data["cursorBrightness"].toDouble(&ok);
    if(ok) setCursorBrightness(brightness);
    //    auto size = data["cursorSize"].toDouble(&ok);
    //    if(ok) setSize(size);
}

void Cursor::paint(uint32_t* pixels)
{
    auto color = monitor()->color();
    monitor()->activePainter()->drawCircle(pixels, m_x, m_y, 1, m_cursorBrightness, color);
}

Cursor *Cursor::clone()
{
    Cursor * cl = new Cursor;
    cl->setCursorBrightness(this->cursorBrightness());
    cl->setMonitor(this->monitor());
    cl->setSize(this->size());
    cl->setX(this->x());
    cl->setY(this->y());
    cl->setParent(this->parent());
    return cl;
}

void MGKAbstractMonitor::updateAfterShining()
{
    for (int i = height() - 1; i >= 0; --i) {
        auto line = drawImage.scanLine(i);
        for (int j = 3; j < drawImage.bytesPerLine(); j += 4) {
            const auto k = (line[j] > 0.7 * 255) ? 0.9/* * KMath::dmap(m_brightness, 0, 1, 0, 0.9)*/ : (line[j] > 0.3 * 255) ? 0.8/*KMath::dmap(m_brightness, 0, 1, 0, 0.8)*/ : 0.9;
            line[j] = static_cast<unsigned char>(line[j] * k * afterShiningIntencity());
//            else if  line[j] = static_cast<unsigned char>(line[j] * KMath::dmap(m_brightness, 0, 1, 0, 0.8));
//            else line[j] = static_cast<unsigned char>(line[j] * KMath::dmap(m_brightness, 0, 1, 0, 0.9));
        }
    }
}

//void MGKAbstractMonitor::onSearchScaleChanged(SearchScale)
//{

//}
