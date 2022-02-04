#include "segmentitem.h"

#include <QSGFlatColorMaterial>
#include <QSGNode>
#include <math.h>

SegmentItem::SegmentItem()
{
    setFlag(ItemHasContents, true);
    connect(this, &SegmentItem::pointsChanged, this, [this](){
        this->update();
    });

    connect(this, &SegmentItem::lineWidthChanged, this, [this](){
        this->update();
    });

    connect(this, &SegmentItem::colorChanged, this, [this](){
        this->update();
    });
}

QSGNode *SegmentItem::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *)
{
    QSGGeometryNode *n = static_cast<QSGGeometryNode*>(node);

    if (!n)
    {
        n = new QSGGeometryNode();
        //matherial
        int l = points().count();
        auto g = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        g->setDrawingMode(QSGGeometry::DrawLineStrip);
        g->allocate(l , 0);

        // color
        auto m = new QSGFlatColorMaterial();
        m->setColor(QColor(color()));

        n->setGeometry(g);
        n->setMaterial(m);
        return n;
    }

    QVector<QPointF> segmentPoints;

    if (points().count() > 2)
    {
        for (int i = 0; i < points().count(); i++)
        {
            if (i == 0) segmentPoints.push_back(points().at(i));
            //if (i == segmentPoints.count() - 1) {}/*segmentPoints.push_back(points.last());*/
            else if (i > 0)
            {
                QPointF pI = points().at(i);
                QPointF pI1 = points().at(i-1);
                double c = 5 / (sqrt( pow(pI.x() - pI1.x(), 2) + pow(pI.y() - pI1.y(), 2)));
                QPointF result((pI.x() + pI1.x()) / 2 + c * (pI.y() - pI1.y()),
                               (pI.y() + pI1.y()) / 2 - c * (pI.x() - pI1.x()));
                segmentPoints.push_back(result);
            }
        }
        for (int i = points().count() - 1; i  >= 0; i--)
        {
            if (i == 0) { segmentPoints.push_back(points().at(i)); }
            //if (i == segmentPoints.count() - 1){} /* segmentPoints.push_back(points.last());*/
            else if (i > 0)
            {
                QPointF pI = points().at(i);
                QPointF pI1 = points().at(i-1);
                double c = 5 / (sqrt( pow(pI.x() - pI1.x(), 2) + pow(pI.y() - pI1.y(), 2)));
                QPointF result((pI.x() + pI1.x()) / 2 - c * (pI.y() - pI1.y()),
                               (pI.y() + pI1.y()) / 2 + c * (pI.x() - pI1.x()));
                segmentPoints.push_back(result);
            }
        }
    }

    auto m = new QSGFlatColorMaterial();
    m->setColor(QColor(color()));
    n->setMaterial(m);
    auto g = n->geometry();
    g->setLineWidth(lineWidth());
    int l = segmentPoints.count();
    if (g->vertexCount() != l)
    {
        g->allocate(segmentPoints.count(), 0);
    }

    for (int i=0; i<l; ++i) {
        auto p = segmentPoints.at(i);
        g->vertexDataAsPoint2D()[i].set(p.x(), p.y());
        //qDebug() << p;
    }
    //g->markVertexDataDirty();
    n->markDirty(QSGNode::DirtyGeometry);

    return n;
}
