#include "monitorpainter.h"
#include <math/kmath.h>
#include <QSharedPointer>
#include <QQuickItemGrabResult>
#include <QPainter>
#include <math.h>
#include <QRgb>

MGKAbstractMonitor *MonitorPainter::monitor() const
{
    return m_monitor;
}

void MonitorPainter::setMonitor(MGKAbstractMonitor *monitor)
{

    m_monitor = monitor;

}

uint32_t MonitorPainter::getPixel(uint32_t *pixels, int x, int y)
{
    if (x >= 0 && y >= 0 && x < m_imW && y < m_imH && monitor()->mask.pixel(x, y) != 0)
        return pixels[y * m_imW + x];
    return 0;
}

MonitorPainter::MonitorPainter(MGKAbstractMonitor *parent) : QObject(parent)
{
    setMonitor(parent);
    setImW(parent->width());
    setImH(parent->height());
    connect(parent, &MGKAbstractMonitor::sizeChanged, this, [this](){
        setImW(m_monitor->width());
        setImH(m_monitor->height());
    });
}

void MonitorPainter::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);
}
//int alpha = static_cast<int>(std::min(pressure * 255 * monitor()->brightness(), 255.));

void MonitorPainter::drawPixel(uint32_t * pixels, int px, int py, double alpha, const QColor & color)
{
    if (px >= 0 && py >= 0 && px < m_imW && py < m_imH && monitor()->mask.pixel(px, py) != 0) {
        pixels[py * m_imW + px] = qRgba(color.red(), color.green(), color.blue(), alpha * 255);
    }
}

void MonitorPainter::drawFilledCircle(uint32_t * pixels, double cx, double cy, double radius, double alpha, const QColor & color)
{
    for(int i = 0; i < sqrt(radius * radius - i * i); i++) {
        int i2 = static_cast<int>(sqrt(radius * radius - i * i));
        drawPixel(pixels, cx + i, cy + i2, alpha, color);
        drawPixel(pixels, cx - i, cy - i2, alpha, color);
        drawPixel(pixels, cx + i, cy - i2, alpha, color);
        drawPixel(pixels, cx - i, cy + i2, alpha, color);

        drawPixel(pixels, cx + i2, cy + i, alpha, color);
        drawPixel(pixels, cx - i2, cy - i, alpha, color);
        drawPixel(pixels, cx - i2, cy + i, alpha, color);
        drawPixel(pixels, cx + i2, cy - i, alpha, color);
    }
}

void MonitorPainter::drawCircle(uint32_t *pixels, double cx, double cy, double radius, double alpha, const QColor & color)
{
    int x = radius;
    int y = 0;
    int xChange = 1 - ((int)radius << 1);
    int yChange = 0;
    int radiusError = 0;

    while (x >= y)
    {
        for (int i = cx - x; i <= cx + x; i++)
        {
            drawPixel(pixels, i, cy + y, alpha, color);
            drawPixel(pixels, i, cy - y, alpha, color);
        }
        for (int i = cx - y; i <= cx + y; i++)
        {
            drawPixel(pixels, i, cy + x, alpha, color);
            drawPixel(pixels, i, cy - x, alpha, color);
        }

        y++;
        radiusError += yChange;
        yChange += 2;
        if (((radiusError << 1) + xChange) > 0)
        {
            x--;
            radiusError += xChange;
            xChange += 2;
        }
    }
}

//void MonitorPainter::drawCircle(double cx, double cy, double radius, QPainter *painter, double pressure, QColor color)
//{
//    int alpha = KMath::instance()->dmap(std::min(pressure * monitor()->brightness() * 0.5, 1.0), 0, 1, 0, 255);
//    color.setAlpha(alpha);
//    painter->setBrush(QBrush(color));
//    painter->drawEllipse(cx, cy, radius, radius);
//}

void MonitorPainter::drawPrettyLine(uint32_t * pixels, int point1X, int point1Y, int point2X, int point2Y, double alpha, double width, QColor color, QColor backgroundColor)
{
    // Calculate line params
    int dx = (point2X - point1X);
    int dy = (point2Y - point1Y);
    int temp;
    float k;

    // Set start pixel
    drawPixel(pixels, point1X, point1Y, alpha, color);

    // X-dominant line
    if (abs(dx) > abs(dy))
    {
        // Ex-change line end points
        if (dx < 0)
        {

            temp = point1X;
            point1X = point2X;
            point2X = temp;

            temp = point1Y;
            point1Y = point2Y;
            point2Y = temp;
        }
        k = (float)dy / (float)dx;

        // Set middle pixels
        int xs;
        float yt = (float)point1Y + k;
        float distance;
        unsigned char red, green, blue;
        for (xs = point1X + 1; xs < point2X; xs++)
        {
            distance = (float)(yt - (int)(yt));
            auto bgColor = pixel_to_RGBA(getPixel(pixels, xs, yt));
            red = (unsigned char)(distance * bgColor.r) + (unsigned char)((1.0f-distance)*color.red());
            green = (unsigned char)(distance * bgColor.g) + (unsigned char)((1.0f-distance)*color.green());
            blue = (unsigned char)(distance * bgColor.b) + (unsigned char)((1.0f-distance)*color.blue());
            drawPixel(pixels, xs, (int)yt, alpha, QColor(red,green,blue));

            bgColor = pixel_to_RGBA(getPixel(pixels, xs, yt + 1));
            red = (unsigned char)((1.0f-distance) * bgColor.r) + (unsigned char)(distance * color.red());
            green = (unsigned char)((1.0f-distance) * bgColor.g) + (unsigned char)(distance * color.green());
            blue = (unsigned char)((1.0f-distance) * bgColor.b) + (unsigned char)(distance*color.blue());
            drawPixel(pixels, xs, (int)yt+1, alpha, QColor(red,green,blue));

            yt += k;
        }
    }
    // Y-dominant line
    else
    {
        // Ex-change line end points
        if (dy < 0)
        {
            temp = point1X;
            point1X = point2X;
            point2X = temp;

            temp = point1Y;
            point1Y = point2Y;
            point2Y = temp;
        }
        k = (float)dx / (float)dy;

        // Set middle pixels
        int ys;
        float xt = (float)point1X + k;
        float distance;
        unsigned char red, green, blue;
        for (ys = point1Y + 1; ys < point2Y; ys++)
        {
            distance = (float)(xt - (int)(xt));

//            bgColor = ::GetPixel(hDC, (int)xt, ys);
            auto bgColor = pixel_to_RGBA(getPixel(pixels, xt, ys));
            red = (unsigned char)(distance*bgColor.r) + (unsigned char)((1.0f-distance)    *color.red());
            green = (unsigned char)(distance*bgColor.g) + (unsigned char)((1.0f-distance)*color.green());
            blue = (unsigned char)(distance*bgColor.b) + (unsigned char)((1.0f-distance)  *color.blue());
            drawPixel(pixels, (int)xt, ys, alpha, QColor(red,green,blue));

            bgColor = pixel_to_RGBA(getPixel(pixels, xt+1, ys));
            red = (unsigned char)((1.0f-distance)*bgColor.r) + (unsigned char)(distance*color.red());
            green = (unsigned char)((1.0f-distance)*bgColor.g) + (unsigned char)(distance*color.green());
            blue = (unsigned char)((1.0f-distance)*bgColor.b) + (unsigned char)(distance*color.blue());
            drawPixel(pixels, (int)xt+1, ys, alpha, QColor(red,green,blue));

            xt += k;
        }
    }

    // Set end pixel
    drawPixel(pixels, point2X, point2Y, alpha, color);

}

void MonitorPainter::drawPrettyPoligon(uint32_t * pixels, int point1X, int point1Y, double pressure)
{
    if(poligonStarted) {
        drawPrettyLine(pixels, lastX, lastY, point1X, point1Y, pressure, 2);
    }
    poligonStarted = true;
    lastX = point1X;
    lastY = point1Y;
}

void MonitorPainter::drawPrettyPoint(uint32_t * pixels, int pointX, int pointY, double alpha, double width)
{
//    int alpha = KMath::instance()->dmap(std::min(pressure * monitor()->brightness() * 0.5, 1.0), 0, 1, 0, 255);
    drawFilledCircle(pixels, pointX, pointY, alpha, width);
}

void MonitorPainter::endPoligon()
{
    poligonStarted = false;
}

//void MonitorPainter::drawDashedLine(double p1x, double p1y, double p2x, double p2y, QPainter *painter, double width, double spacing, QColor color)
//{
////    int r_px = this->width() / 2.0;
////    int r_py = this->height() / 2.0;
//    auto line = QVector2D(p2x, p2y) - QVector2D(p1x, p1y);
//    auto lineLength = line.length();
//    auto pointsCount = (int)(2 * ((lineLength - spacing) / (2 * spacing))) + 2;

//    for (int i = 0, size = pointsCount; i < size; i+=2) {
//        drawPrettyLine(p2x - i * spacing * line.normalized().x(), p1y + i * spacing * line.normalized().y(), p2x - (i + 1) * spacing * line.normalized().x(), p1y + (i + 1) * spacing * line.normalized().y(), painter, 1, width, color);
//    }
//}

RGBA pixel_to_RGBA(const uint32_t u32) {
    // To extract each byte, we can mask them using bitwise AND (&)
    // then shift them right to the first byte.
    RGBA rgba;
    rgba.a = (u32 & 0xff000000) >> 24;
    rgba.r = (u32 & 0x00ff0000) >> 16;
    rgba.g = (u32 & 0x0000ff00) >> 8;
    rgba.b =  u32 & 0x000000ff;
    return rgba;
}

void MonitorPainter::WuDrawLine(uint32_t * pixels, double x0, double y0, double x1, double y1, double alpha, const QColor & color) {
    auto ipart = [](double x) -> int {return int(std::floor(x));};
    auto round = [](double x) -> double {return std::round(x);};
    auto fpart = [](double x) -> double {return x - std::floor(x);};
    auto rfpart = [=](double x) -> double {return 1 - fpart(x);};

    const bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        std::swap(x0,y0);
        std::swap(x1,y1);
    }
    if (x0 > x1) {
        std::swap(x0,x1);
        std::swap(y0,y1);
    }

    const float dx = x1 - x0;
    const float dy = y1 - y0;
    const float gradient = (dx == 0) ? 1 : dy/dx;

    int xpx11;
    float intery;
    {
        const float xend = round(x0);
        const float yend = y0 + gradient * (xend - x0);
        const float xgap = rfpart(x0 + 0.5);
        xpx11 = int(xend);
        const int ypx11 = ipart(yend);
        if (steep) {
            drawPixel(pixels, ypx11,     xpx11, rfpart(yend) * xgap * alpha, color);
            drawPixel(pixels, ypx11 + 1, xpx11,  fpart(yend) * xgap * alpha, color);
        } else {
            drawPixel(pixels, xpx11, ypx11,    rfpart(yend) * xgap * alpha, color);
            drawPixel(pixels, xpx11, ypx11 + 1, fpart(yend) * xgap * alpha, color);
        }
        intery = yend + gradient;
    }

    int xpx12;
    {
        const float xend = round(x1);
        const float yend = y1 + gradient * (xend - x1);
        const float xgap = rfpart(x1 + 0.5);
        xpx12 = int(xend);
        const int ypx12 = ipart(yend);
        if (steep) {
            drawPixel(pixels, ypx12,     xpx12, rfpart(yend) * xgap * alpha, color);
            drawPixel(pixels, ypx12 + 1, xpx12,  fpart(yend) * xgap * alpha, color);
        } else {
            drawPixel(pixels, xpx12, ypx12,    rfpart(yend) * xgap * alpha, color);
            drawPixel(pixels, xpx12, ypx12 + 1, fpart(yend) * xgap * alpha, color);
        }
    }

    if (steep) {
        for (int x = xpx11 + 1; x < xpx12; x++) {
            drawPixel(pixels, ipart(intery),     x, rfpart(intery) * alpha, color);
            drawPixel(pixels, ipart(intery) + 1, x,  fpart(intery) * alpha, color);
            intery += gradient;
        }
    } else {
        for (int x = xpx11 + 1; x < xpx12; x++) {
            drawPixel(pixels, x, ipart(intery),     rfpart(intery) * alpha, color);
            drawPixel(pixels, x, ipart(intery) + 1,  fpart(intery) * alpha, color);
            intery += gradient;
        }
    }
}
