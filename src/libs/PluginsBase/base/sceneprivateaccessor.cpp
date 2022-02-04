#include "sceneprivateaccessor.h"
#include <base/scenebase.h>
#include <QJsonDocument>

ScenePrivateAccessor::ScenePrivateAccessor(QObject *parent) : QObject(parent)
{

}

void ScenePrivateAccessor::setScene(SceneBase *scene)
{
    m_scene = scene;
    if (m_scene == nullptr){
        m_factoriesHash      = nullptr;
        m_sceneItemsList     = nullptr;
        m_factoriesList      = nullptr;
        m_createdControllers = nullptr;
    }else{
        m_factoriesHash = &scene->factoriesHash;
        m_sceneItemsList = &scene->sceneItemsList;
        m_factoriesList = &scene->factoriesList;
        m_createdControllers = &scene->createdControllers;
    }
}

