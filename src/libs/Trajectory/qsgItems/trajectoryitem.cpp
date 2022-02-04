#include "trajectoryitem.h"

#include <QSGNode>
#include <QSGGeometryNode>
#include <QSGFlatColorMaterial>
#include <QPointF>
#include <QVector>
#include <traceData/trajectory.h>

TrajectoryItem::TrajectoryItem(QQuickItem *parent) : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);

    connect(this, &TrajectoryItem::trajectoryChanged, this, [this](){
        connect(trajectory(), &Trajectory::vertexesChanged, this, &TrajectoryItem::update);
    });
}

QSGNode *TrajectoryItem::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *)
{
    if (node == nullptr) {
        node = new QSGNode;
    }

    clearChilds(node);

    for (const auto &b : trajectory()->vertexes()) {
        auto n = new QSGGeometryNode();

        if (b->type() == "Circle")
        {
            auto s = qobject_cast<CircleSegment*>(b);
            buildCircle(n, s);
        }
        else if (b->type() == "Line")
        {
            auto s = qobject_cast<LineSegment*>(b);
            buildLine(n, s);
        }
        else if (b->type() == "Torpedo")
        {
            auto s = qobject_cast<FireSegment*>(b);
            buildFire(n, s);
        }
        node->appendChildNode(n);
    }
    node->markDirty(QSGNode::DirtyGeometry);
    return node;
}

void TrajectoryItem::buildLine(QSGGeometryNode *n, LineSegment *segment)
{
    auto g = n->geometry();
    if  (g == nullptr) {
        g = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2);
        g->setDrawingMode(QSGGeometry::DrawLineStrip);
        // color
        auto m = new QSGFlatColorMaterial();
        m->setColor(segment->color());
        //node
        n->setGeometry(g);
        n->setMaterial(m);
    }
    static_cast<QSGFlatColorMaterial*>(n->material())->setColor(segment->selected() ? segment->selectedColor() : segment->color());

    g->allocate(2);
    g->vertexDataAsPoint2D()[0].set(segment->points()[0].x(), Y_SWAP*segment->points()[0].y());
    g->vertexDataAsPoint2D()[1].set(segment->points()[1].x(), Y_SWAP*segment->points()[1].y());
    n->markDirty(QSGNode::DirtyGeometry);

    if (segment->selected()) {
        auto sg = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2);
        sg->setDrawingMode(QSGGeometry::DrawPoints);
        sg->setLineWidth(5);
        // color
        auto sm = new QSGFlatColorMaterial();
        sm->setColor(segment->selected() ? segment->selectedColor() : segment->color());
        //node
        auto sn = new QSGGeometryNode;
        sn->setGeometry(sg);
        sn->setMaterial(sm);

        sg->allocate(2);
        sg->vertexDataAsPoint2D()[0].set(segment->points()[0].x(), Y_SWAP*segment->points()[0].y());
        sg->vertexDataAsPoint2D()[1].set(segment->points()[1].x(), Y_SWAP*segment->points()[1].y());
        sn->markDirty(QSGNode::DirtyGeometry);

        n->appendChildNode(sn);
    }
}

void TrajectoryItem::buildFire(QSGGeometryNode *n, FireSegment *segment)
{
    auto g = n->geometry();
    if  (g == nullptr) {
        g = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2);
        g->setDrawingMode(QSGGeometry::DrawLineStrip);
        // color
        auto m = new QSGFlatColorMaterial();
        m->setColor(segment->color());
        //node
        n->setGeometry(g);
        n->setMaterial(m);
    }
    static_cast<QSGFlatColorMaterial*>(n->material())->setColor(segment->selected() ? segment->selectedColor() : segment->color());

    g->allocate(2);
    g->vertexDataAsPoint2D()[0].set(segment->points()[0].x(), Y_SWAP*segment->points()[0].y());
    g->vertexDataAsPoint2D()[1].set(segment->points()[1].x(), Y_SWAP*segment->points()[1].y());
    n->markDirty(QSGNode::DirtyGeometry);

    if (segment->selected()) {
        auto sg = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2);
        sg->setDrawingMode(QSGGeometry::DrawPoints);
        sg->setLineWidth(5);
        // color
        auto sm = new QSGFlatColorMaterial();
        sm->setColor(segment->selected() ? segment->selectedColor() : segment->color());
        //node
        auto sn = new QSGGeometryNode;
        sn->setGeometry(sg);
        sn->setMaterial(sm);

        sg->allocate(2);
        sg->vertexDataAsPoint2D()[0].set(segment->points()[0].x(), Y_SWAP*segment->points()[0].y());
        sg->vertexDataAsPoint2D()[1].set(segment->points()[1].x(), Y_SWAP*segment->points()[1].y());
        sn->markDirty(QSGNode::DirtyGeometry);

        n->appendChildNode(sn);
    }
}

void TrajectoryItem::buildCircle(QSGGeometryNode *n, CircleSegment* segment)
{
    auto g = n->geometry();
    int l = segment->points().count();
    if  (g == nullptr) {
        g = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), l);
        g->setLineWidth(1);
        g->setDrawingMode(QSGGeometry::DrawLineStrip);
        // color
        auto m = new QSGFlatColorMaterial();
        m->setColor(segment->color());
        //node
        n->setGeometry(g);
        n->setMaterial(m);
    }
    static_cast<QSGFlatColorMaterial*>(n->material())->setColor(segment->selected() ? segment->selectedColor() : segment->color());

    auto nn = segment->points().count() + 1 + (segment->selected() ? 2 : 0);
    g->allocate(nn);
    if (segment->selected()) g->vertexDataAsPoint2D()[0].set(segment->c().x(), Y_SWAP*segment->c().y());
    for (int  i = 0; i <= segment->points().count(); ++i) {
        if (i == segment->points().count()) g->vertexDataAsPoint2D()[i+(segment->selected()?1:0)].set(segment->points()[i - 1].x(), segment->points()[i - 1].y());
        else g->vertexDataAsPoint2D()[i+(segment->selected()?1:0)].set(segment->points()[i].x(), segment->points()[i].y());
    }

    if (segment->selected()) g->vertexDataAsPoint2D()[nn - 1].set(segment->c().x(), Y_SWAP*segment->c().y());
    n->markDirty(QSGNode::DirtyGeometry);

    if (segment->selected()) {
        auto sg = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 2);
        sg->setDrawingMode(QSGGeometry::DrawPoints);
        sg->setLineWidth(5);
        // color
        auto sm = new QSGFlatColorMaterial();
        sm->setColor(segment->selected() ? segment->selectedColor() : segment->color());
        //node
        auto sn = new QSGGeometryNode;
        sn->setGeometry(sg);
        sn->setMaterial(sm);

        sg->allocate(3);
        sg->vertexDataAsPoint2D()[0].set(g->vertexDataAsPoint2D()[1].x, g->vertexDataAsPoint2D()[1].y);
        sg->vertexDataAsPoint2D()[1].set(g->vertexDataAsPoint2D()[l+1].x, g->vertexDataAsPoint2D()[l+1].y);
        sg->vertexDataAsPoint2D()[2].set(segment->c().x(), Y_SWAP*segment->c().y());
        sn->markDirty(QSGNode::DirtyGeometry);

        n->appendChildNode(sn);
    }
}

void TrajectoryItem::clearChilds(QSGNode *n)
{
    for (int i = n->childCount() - 1; i >= 0; i--)
    {
        auto ch = n->childAtIndex(i);
        n->removeChildNode(ch);
        delete ch;
    }
}
