#ifndef KGRIDBACKGROUND_H
#define KGRIDBACKGROUND_H

#include <QQuickItem>
#include <libs/klibcorelite/kmacro.h>
#include <QPointF>
#include <QSGGeometryNode>

class KGridBackground : public QQuickItem
{
    Q_OBJECT
    K_QML_TYPE(KGridBackground)
    K_AUTO_PROPERTY(double, sceneWidth, sceneWidth, setSceneWidth, sceneWidthChanged, 0)
    K_AUTO_PROPERTY(double, sceneHeight, sceneHeight, setSceneHeight, sceneHeightChanged, 0)
    K_AUTO_PROPERTY(double, scaleFactor, scaleFactor, setScaleFactor, scaleFactorChanged, 0)
    K_AUTO_PROPERTY(QPointF, leftCorner, leftCorner, setLeftCorner, leftCornerChanged, QPointF(0,0))
public:
    KGridBackground();

protected:
    virtual QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *) override;
    virtual void geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) override;
private:
    bool m_geometryChanged = true;

};

#endif // KGRIDBACKGROUND_H
