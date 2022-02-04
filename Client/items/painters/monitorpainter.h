#ifndef MONITORPAINTER_H
#define MONITORPAINTER_H

#include <QObject>

#include <items/mgkabstractmonitor.h>

class MonitorPainter : public QObject
{
    Q_OBJECT

    K_PROTECTED_PROPERTY(int , imW, imW, setImW, imWChanged, 0)
    K_PROTECTED_PROPERTY(int , imH, imH, setImH, imHChanged, 0)

    MGKAbstractMonitor * m_monitor = nullptr;
    bool poligonStarted = false;
    int lastX = 0;
    int lastY = 0;
//protected:
//    int imW, imH;


public:
    explicit MonitorPainter(MGKAbstractMonitor *parent = nullptr);

    virtual void paint(QPainter *painter);

    void drawPixel(uint32_t * pixels, int px, int py, double alpha = 1.0, const QColor &color = "#ffff00");
    uint32_t getPixel(uint32_t * pixels, int x, int y);
    void drawFilledCircle(uint32_t *pixels, double cx, double cy, double radius, double pressure = 1.0, const QColor &color = "#ffff00");
    void drawCircle(uint32_t * pixels, double cx, double cy, double radius, double alpha = 1.0, const QColor &color = "#ffff00");
    void drawPrettyLine(uint32_t * pixels, int point1X, int point1Y, int point2X, int point2Y, double alpha = 1.0, double width = 1.0, QColor color = "#ffff00", QColor backgroundColor = "#000000");
    void drawPrettyPoligon(uint32_t *pixels, int point1X, int point1Y, double pressure = 1.0);
    void drawPrettyPoint(uint32_t * pixels, int pointX, int pointY, double alpha = 1.0, double width = 1);
    void endPoligon();

    void drawDashedLine(double p1x, double p1y, double p2x, double p2y, QPainter * painter, double width = 1.0, double spacing = 4.0, QColor color = "#ffff00");

    void WuDrawLine(uint32_t *pixels, double x0, double y0, double x1, double y1, double alpha, const QColor &color);


    MGKAbstractMonitor *monitor() const;
    void setMonitor(MGKAbstractMonitor *monitor);

signals:

};

struct RGBA {
    int r, g, b, a;
};

RGBA pixel_to_RGBA(const uint32_t u32);


#endif // MONITORPAINTER_H
