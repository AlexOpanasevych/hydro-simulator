#include "ksceneprovider.h"
#include <scene/kscene.h>


KSceneProvider::KSceneProvider(QObject *parent) : QObject(parent)
{

}

KScene *KSceneProvider::getScene(QString sceneID, QPointF size)
{
    KScene *result {};

    if (m_scenes[sceneID])
    {
        result = m_scenes[sceneID];
        result->setSceneWidth(size.x());
        result->setSceneHeight(size.y());
    }
    else
    {
        result = new KScene(this);
        result->setSceneWidth(size.x());
        result->setSceneHeight(size.y());
        m_scenes.insert(sceneID, result);
    }
    return result;
}

KScene *KSceneProvider::getScene(QString sceneID, double width, double height)
{
    return getScene(sceneID, QPointF(width, height));
}
