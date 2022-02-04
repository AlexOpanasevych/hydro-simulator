#ifndef KBACKGROUND_H
#define KBACKGROUND_H

#include <QSGGeometryNode>
#include <QQuickWindow>

class KBackground : public QSGGeometryNode
{
public:
    KBackground(QQuickWindow *window);

    void setRect(const QRectF &bounds);
private:
    QQuickWindow *window = nullptr;
};

#endif // KBACKGROUND_H
