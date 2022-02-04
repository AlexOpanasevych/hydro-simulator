#include "xnleaf.h"
#include <math.h>

XNLeaf::XNLeaf(): m_geometry(new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4))
{
    setGeometry(m_geometry);
    m_material = MinimalShader::createMaterial();
    qreal r, g, b, a;
    color.getRgbF(&r,&g, &b, &a);
    m_material->state()->r = r;
    m_material->state()->g = g;
    m_material->state()->b = b;
    m_material->state()->a = a;
    m_material->setFlag(QSGMaterial::Blending);
    m_geometry->setDrawingMode(GL_POLYGON);
    setMaterial(m_material);
    setFlags(QSGNode::OwnsMaterial);
}

void XNLeaf::updatePoints()
{
    const int n = 20;
    double massR[n+1];
    /* W -> coefficient for cos(5 * w)=0.8 */
    //auto w = 7.37398;
    auto w = acos(0.8) / (arcAngle * M_PI / 180);
    double step = M_PI / (w * 2 * n);

    QVector<QPointF> points;

    for(int i = 0; i <= n; i++) {
        auto phi = i * step;
        //phi *= w;
        massR[i] = maxLength * cos(w * phi);
        points.append({ massR[i] * sin(phi), -massR[i] * cos(phi)});
    }

    for(int i = points.size() - 1; i >= 0; i--){
        points.append({-points.at(i).x(), points.at(i).y()});
    }

    m_geometry->allocate(points.count(), 0);

    for (int i = 0; i < points.size(); i++){
        QPointF p = rotate(points[i], angle);
        m_geometry->vertexDataAsPoint2D()[i].set(p.x(), p.y());
    }
    markDirty(QSGNode::DirtyGeometry | QSGNode::DirtyMaterial);
}

void XNLeaf::updateColor()
{
     qreal r, g, b, a;
    color.getRgbF(&r,&g, &b, &a);
    m_material->state()->r = r;
    m_material->state()->g = g;
    m_material->state()->b = b;
    m_material->state()->a = a;
    markDirty(QSGNode::DirtyMaterial);
}

QPointF XNLeaf::rotate(QPointF old, double angle)
{
    QPointF p = old;
    double radius = 0, pAngle = 0;
    //! translate to polar
    radius = sqrt(pow(old.x(), 2) + pow(old.y(), 2));
    pAngle = atan(old.x() / old.y()) * 180 / M_PI + angle - 90;

    p.setX(cos(pAngle * M_PI / 180) * radius);
    p.setY(sin(pAngle * M_PI / 180) * radius);
    return p;
}
