#include "mgkrectmonitorprivate.h"

#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGGeometryNode>
#include <QSGSimpleRectNode>
#include <cmath>
#include <QPainter>

#include <items/painters/csnrpainter.h>
#include <items/painters/doplerpainter.h>
#include <items/painters/epo1painter.h>


MGKRectMonitorPrivate::MGKRectMonitorPrivate(QQuickItem *parent) : MGKAbstractMonitor(parent)
{
    connect(this, &MGKAbstractMonitor::sizeChanged, this, &MGKRectMonitorPrivate::onSizeChanged);
    connect(this, &MGKAbstractMonitor::epModeChanged, this, &MGKRectMonitorPrivate::onModeChanged);
    onModeChanged();
}

void MGKRectMonitorPrivate::paint(QPainter *painter)
{
    painter->setBrush(backgroundColor());
    painter->drawRect(boundingRect());
    MGKAbstractMonitor::paint(painter);
}

void MGKRectMonitorPrivate::onSizeChanged()
{
    int w = width();
    int h = height();
    QImage mask(w, h, QImage::Format_ARGB32);
    mask.fill(0);
    if(!mask.isNull()) {
        QPainter maskPainter(&mask);
        maskPainter.setBrush(Qt::darkGray);
//        maskPainter.setPen(QPen(Qt::darkGray, 0));
        maskPainter.drawRect(boundingRect());
    }
    this->mask = mask;
}

void MGKRectMonitorPrivate::onMonitorWorkChanged()
{
    if(!monitorWork()) {
        if(activePainter()) activePainter()->deleteLater();
        if(screenCursor()) screenCursor()->deleteLater();
    }
    else {

    }
}

void MGKRectMonitorPrivate::onModeChanged()
{
    if(m_activePainter) m_activePainter->deleteLater();
    if(m_screenCursor) m_screenCursor->deleteLater();
    switch(epMode()) {
    case EpO1Mode::O1:
        [[fallthrough]];
    case EpO1Mode::O2:
        [[fallthrough]];
    case EpO1Mode::S1:
        [[fallthrough]];
    case EpO1Mode::S2:
        setActivePainter(new SweepPainter(this));
        setScreenCursor(new Cursor(this));
        break;
    case EpO1Mode::Shp:
        setActivePainter(new CSNRPainter(this));
        setScreenCursor(new LineCircleCursor(this));
        break;
    }
}

//QSGNode *MGKRectMonitorPrivate::updatePaintNode(QSGNode * oldNode, QQuickItem::UpdatePaintNodeData * )
//{
//    QSGSimpleRectNode * node = nullptr;
//    if(!oldNode) {
//        node = new QSGSimpleRectNode;
//        node->setRect(boundingRect());
//        node->setColor(backgroundColor());

//        QSGNode * afterShiningNode = new QSGNode;
////        {

////            QSGGeometry* geometry = new QSGGeometry(QSGGeometry::defaultAttributes_ColoredPoint2D(), boundingRect().height() * 2);
////            geometry->setDrawingMode(QSGGeometry::DrawLines);

////            QSGFlatColorMaterial* material = new QSGFlatColorMaterial;
//////            m_color.setAlphaF(0.0);
//////            colorChanged(m_color);
////            material->setColor(color());

////            afterShiningNode->setGeometry(geometry);
////            afterShiningNode->setFlag(QSGNode::OwnsGeometry);
////            afterShiningNode->setMaterial(material);
////            afterShiningNode->setFlag(QSGNode::OwnsMaterial);
////        }
//        node->appendChildNode(afterShiningNode);
//        setPaintNode(afterShiningNode);
//    }
//    else {
//        node = dynamic_cast<QSGSimpleRectNode*>(oldNode);
//        setPaintNode(node->lastChild());
//        updateSurface();

//    }

//    return node;
//}

//void MGKRectMonitorPainter::synchronize(QNanoQuickItem *item)
//{
//    auto monitorItem = static_cast<MGKRectMonitorPrivate*>(item);
//    if(monitorItem->color() != color) {
//        color = monitorItem->color().name();
//    }
//    if(monitorItem->backgroundColor() != backgroundColor) {
//        backgroundColor = monitorItem->backgroundColor().name();
//    }
//}

//void MGKRectMonitorPainter::paint(QNanoPainter *painter)
//{
//    painter->setFillStyle(QNanoColor(backgroundColor));
//    painter->beginPath();
//    painter->rect(0, 0, height(), width());
//    painter->fill();


//}
