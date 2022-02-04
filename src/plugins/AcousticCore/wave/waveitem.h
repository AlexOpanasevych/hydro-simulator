#ifndef WAVEITEM_H
#define WAVEITEM_H

#include <QSGFlatColorMaterial>
#include <QSGNode>
#include <resources/simplematerial.h>
#include <resources/gradientmaterial.h>
#include <QSGSimpleMaterial>

class WaveItem : public QSGGeometryNode
{
public:
    WaveItem();
    ~WaveItem(){}
    QColor m_color = QColor("red");
    void setPoints(const QVector<QPointF> &points);
private:
    QSGSimpleMaterial<Color> *m_material = nullptr;
    QSGGeometry *m_geometry = nullptr;
};

#endif // WAVEITEM_H
