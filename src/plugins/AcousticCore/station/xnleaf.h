#ifndef XNLEAF_H
#define XNLEAF_H

#include <QSGGeometryNode>
#include <resources/simplematerial.h>

class XNLeaf: public QSGGeometryNode
{
public:
    XNLeaf();
    double angle = 30;
    double arcAngle = 5;
    double maxLength = 400;
    QColor color = "red";
    void updatePoints();
    void updateColor();
private:
    QSGSimpleMaterial<Color> *m_material = nullptr;
    QSGGeometry *m_geometry = nullptr;
    QPointF rotate(QPointF old, double angle);
};

#endif // XNLEAF_H
