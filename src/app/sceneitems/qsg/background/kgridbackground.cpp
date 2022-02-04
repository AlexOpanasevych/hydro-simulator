#include "kgridbackground.h"
#include <QSGFlatColorMaterial>
#include <QSGGeometry>
#include <QSGNode>
#include <QSGRendererInterface>
#include <sceneitems/qsg/background/kgridnode.h>
#include <sceneitems/qsg/background/kbackground.h>
#include <cdebug.h>

class GraphNode : public QSGNode
{
public:
    KGridNode *grid;
    KBackground *background;
};

KGridBackground::KGridBackground()
{
    setFlag(ItemHasContents, true);

    //connect(this, &KGridBackground::scaleFactorChanged, this, [this](){
    //   qDebug(v) << scaleFactor();
    //});
}

QSGNode *KGridBackground::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *)
{
    GraphNode *n = static_cast<GraphNode *>(node);

    QRectF rect = boundingRect();

    if (rect.isEmpty())
    {
        delete n;
        return nullptr;
    }

    if (!n)
    {
        n = new GraphNode();

        n->grid = new KGridNode();
        n->background = new KBackground(window());

        n->appendChildNode(n->background);
        n->appendChildNode(n->grid);

        m_geometryChanged = true;
    }

    if (m_geometryChanged) {
        n->background->setRect(rect);
        n->grid->setRect(rect);
    }

    m_geometryChanged = false;

    return n;
}

void KGridBackground::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
{
    m_geometryChanged = true;
    update();
    QQuickItem::geometryChanged(newGeometry, oldGeometry);
}
