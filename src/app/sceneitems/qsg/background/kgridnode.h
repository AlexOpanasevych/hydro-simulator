#ifndef KGRIDNode_H
#define KGRIDNode_H

#include <QtQuick/QSGGeometryNode>
#include <QtQuick/QSGFlatColorMaterial>

class KGridNode : public QSGGeometryNode
{
public:
    KGridNode();

    void setRect(const QRectF &rect);

private:
    QSGFlatColorMaterial m_material;
    QSGGeometry m_geometry;
};

#endif // KGridNode_H
