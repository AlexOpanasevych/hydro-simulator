#include "scenetranslater.h"
#include <scene/kscene.h>
#include <sceneitems/cpp/ksceneitem.h>

SceneTranslater::SceneTranslater(QObject *parent) : QObject(parent)
{
    KSceneItem * item = qobject_cast<KSceneItem*>(parent);

    if (item != nullptr)
    {
        setSceneItem(item);
        setScene(item->scene());

        connect(item, &KSceneItem::xChanged, this, [this](double x){
                setSceneX(toSceneCoords(x, 0).x());
        });

        connect(item, &KSceneItem::yChanged, this, [this](double y){
                setSceneY(toSceneCoords(0, y).y());
        });

        connect(item, &KSceneItem::sceneChanged, this, [this](KScene *scene){
           setScene(scene);
        });
    }
}

QPointF SceneTranslater::toSceneCoords(double x, double y)
{
    if (scene() != nullptr)
    {
        double newX = scene()->sceneWidth() / 2 + x;
        double newY = scene()->sceneHeight() / 2 - y;
        return QPointF(newX, newY);
    }
    return QPointF{0, 0};
}

QPointF SceneTranslater::fromSceneCoords(double x, double y)
{
    if (scene() != nullptr)
    {
        double newX =  x - scene()->sceneWidth() / 2;
        double newY =  scene()->sceneHeight() / 2 - y;
        return QPointF(newX, newY);
    }
    return QPointF{0,0};
}
