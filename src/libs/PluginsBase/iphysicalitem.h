#ifndef IPHYSICALITEM_H
#define IPHYSICALITEM_H

#include "AbstractPluginInterface.h"

#include <QString>
#include <QQmlEngine>
#include <QQmlContext>
#include <libs/klibcorelite/kmacro.h>
#include <abstractengine.h>
#include <pluginsbase_global.h>
#include <base/sceneitem.h>
#include <factories/controllerfactory.h>
#include <factories/sceneitemsfactory.h>

class SceneItemsFactory;
class ControllerFactory;

class PLUGINSBASE_EXPORT IPhysicalItem : public AbstractPluginInterface
{
private:
    QList<SceneItemsFactory *> creators;
    QList<ControllerFactory *> controllers;
public:
    IPhysicalItem();
//    virtual QList<QString> sceneItemList() = 0;
    QList<SceneItemsFactory*> getSceneItemsFactories() {return creators;}
//    virtual void init(AbstractEngine* eng) = 0;
    QList<ControllerFactory*> getControllerFactories() {return controllers;}
    virtual QStringList creatableTypes() { return {}; }
    template<class T> void createItemFactory();
    template<class T> void createControllerFactory();
    ~IPhysicalItem();
};

template<class T>
        void IPhysicalItem::createItemFactory()
{
    assert (!typeName().isEmpty());
    auto f = new SceneItemsFactory([]()->SceneItem*{ return new T();});
    f->setObjectName(typeName());
    //TODO : fix path
//    f->setComponentsMap(m);
    creators.push_back(f);
}

template<class T>
        void IPhysicalItem::createControllerFactory()
{
    assert (!typeName().isEmpty());
    auto cf = new ControllerFactory([]()->AbstractPIController*{ return new T();});
    cf->setObjectName(typeName());
    controllers.push_back(cf);
}
Q_DECLARE_INTERFACE(IPhysicalItem, "com.my.IPhysicalItem")

#endif // IPHYSICALITEM_H
