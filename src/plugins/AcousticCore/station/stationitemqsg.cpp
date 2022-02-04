#include "stationitemqsg.h"

#include <QSGNode>


StationItemQSG::StationItemQSG(QQuickItem *parent) : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
}

int StationItemQSG::mode() const
{
    return m_mode;
}

int StationItemQSG::xnCount() const
{
    return m_xnCount;
}

double StationItemQSG::arcAngle() const
{
    return m_arcAngle;
}

double StationItemQSG::startAngle() const
{
    return m_startAngle;
}

void StationItemQSG::setXnCount(int xnCount)
{
    if (m_xnCount == xnCount)
        return;

    m_xnCount = xnCount;
    update();
    emit xnCountChanged(m_xnCount);
}

void StationItemQSG::setArcAngle(double arcAngle)
{
    if (qFuzzyCompare(m_arcAngle, arcAngle))
        return;

    m_arcAngle = arcAngle;
    update();
    emit arcAngleChanged(m_arcAngle);
}

void StationItemQSG::setStartAngle(double startAngle)
{
    if (qFuzzyCompare(m_startAngle, startAngle))
        return;

    m_startAngle = startAngle;
    update();
    emit startAngleChanged(m_startAngle);
}

void StationItemQSG::setMode(int mode)
{
    if (m_mode == mode)
        return;

    m_mode = mode;

    switch (m_mode) {
    case 0:{

        break;
    }
    case 1:{

        break;
    }
    }

    emit modeChanged(m_mode);
}

QSGNode *StationItemQSG::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    if (!oldNode){
        oldNode = new QSGNode();
    }

    int visualXnCount = m_xnList.count();
    if (visualXnCount > xnCount()){
        for (int i = visualXnCount - 1; i >= xnCount(); i--){
            auto w = m_xnList.at(i);
            m_xnList.removeAll(w);
            oldNode->removeChildNode(w);
            oldNode->markDirty(QSGNode::DirtyGeometry);
            delete w;
        }
    }else if (visualXnCount < xnCount()){
        for (int i = 0; i < xnCount() - visualXnCount; i++){
            auto w = new XNLeaf();
            oldNode->appendChildNode(w);
            m_xnList.append(w);
        }
    }
    for (int i = 0; i < xnCount(); i++){
        auto w = m_xnList[i];
        w->color = m_color;
        w->angle = startAngle() + i * arcAngle();
        w->arcAngle = arcAngle() / 2;
        w->updateColor();
        w->updatePoints();
    }
    oldNode->markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);

    return oldNode;
}

QColor StationItemQSG::color() const
{
    return m_color;
}

void StationItemQSG::setColor(QColor newColor)
{
    if (m_color == newColor)
        return;
    m_color = newColor;
    update();
    emit colorChanged();
}
