#include "waveitem.h"
#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGNode>
#include <math.h>
#include <QQmlEngine>
#include <QtGlobal>
#include <resources/simplematerial.h>

WaveItem::WaveItem() : m_geometry(new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4))
{
    setGeometry(m_geometry);
    m_material = MinimalShader::createMaterial();
    qreal r, g, b, a;
    m_color.getRgbF(&r,&g, &b, &a);
    m_material->state()->r = r;
    m_material->state()->g = g;
    m_material->state()->b = b;
    m_material->state()->a = a;
    m_material->setFlag(QSGMaterial::Blending);
    m_geometry->setDrawingMode(GL_TRIANGLES);
    setMaterial(m_material);
    setFlags(QSGNode::OwnsMaterial);
}

void WaveItem::setPoints(const QVector<QPointF> &points)
{
    QSGGeometry *g = geometry();
    g->allocate(points.size(), 0);
    for (int i = 0; i < points.size(); i++){
        g->vertexDataAsPoint2D()[i].set(points[i].x(), points[i].y());
    }
    markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
}
