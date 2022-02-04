#include "traceitem.h"

#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGNode>
#include <QSGRendererInterface>
#include <QQuickWindow>

TraceItem::TraceItem()
{
    setFlag(ItemHasContents, true);

    connect(this, &TraceItem::bufferLengthChanged, this, [this](int length){
        if ( (length < points.length() - 1) && !points.isEmpty())
        points.remove(length, points.length() - 1 - length);
    });
}

void TraceItem::updateCoords(QPointF coords, bool clear )
{
    if (clear) points.clear();
    points.push_front(coords);
    if (points.length() > bufferLength())
        points.pop_back();

    //qDebug() << points.length();
    update();
}

QSGNode *TraceItem::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *)
{
    if (node == nullptr) { //need to create node
        //geometry
        int l = points.count();
        auto g = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 0);
        g->setDrawingMode(QSGGeometry::DrawLineStrip);
        g->allocate(l , 0);

        for (int  i = 0; i < l; ++i) {
            auto p = points.at(i);
            g->vertexDataAsPoint2D()[i].set(p.x(), p.y());
        }
        // color
        auto m = new QSGFlatColorMaterial();
        m->setColor(QColor("black"));
        //node
        auto n = new QSGGeometryNode();
        n->setGeometry(g);
        n->setMaterial(m);
        return n;
    }

    //node already exists -> modify
    auto n = static_cast<QSGGeometryNode*>(node);
    auto m = new QSGFlatColorMaterial();
    m->setColor(QColor("black"));
    n->setMaterial(m);
    auto g = n->geometry();
    g->setLineWidth(1);
    int l = points.count();
    if ((l) != g->vertexCount()) {
        //qDebug() << "alloc";
        g->allocate(l, 0);
    }
    for (int i=0; i<l; ++i) {
        auto p = points.at(i);
        g->vertexDataAsPoint2D()[i].set(p.x(), p.y());
        //qDebug() << p;
    }
    //g->markVertexDataDirty();
    n->markDirty(QSGNode::DirtyGeometry);
    return node;
}
