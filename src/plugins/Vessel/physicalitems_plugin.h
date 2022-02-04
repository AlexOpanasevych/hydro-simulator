#ifndef SCENEITEMS_PLUGIN_H
#define SCENEITEMS_PLUGIN_H

#include <QQmlComponent>
#include <QQmlExtensionPlugin>
#include <libs/PluginsBase/externalInterface.h>
#include <libs/PluginsBase/base/sceneitem.h>
#include <libs/PluginsBase/factories/sceneitemsfactory.h>
#include <libs/PluginsBase/factories/controllerfactory.h>

class PhysicalItemsPlugin : public QQmlExtensionPlugin, public IPhysicalItem
{
    Q_OBJECT
    Q_INTERFACES(IPhysicalItem)
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;

//    virtual QList<QString> sceneItemList() override;
    virtual QList<ControllerFactory *> getControllerFactories() override;
    virtual QList<SceneItemsFactory *> getSceneItemsFactories() override;
    virtual void init(AbstractEngine *engine) override;
    virtual QMap<QString, QMap<QString, QString>> getComponents() override;
    QString typeName() override { return m_typeName; }
private:
    QList<SceneItemsFactory *> creators;
    QList<ControllerFactory *> controllers;
    AbstractEngine * engine = nullptr;
    QString configPath = "../../../bin/plugins/PhysicalItems/content/";
    template<class T>
    void createCreator(QString typeName);
    template<class T>
    void createController(QString typeName);


    const QString m_typeName = "Torpedo";


    const ComponentsMap m {
        {"scene", {
        {"item" , "../../../bin/plugins/PhysicalItems/qml/items/" + m_typeName + "Item.qml"},
        {"menuButton" , "../../../bin/plugins/PhysicalItems/qml/menuButton/" + m_typeName + "MenuButton.qml"},
        {"menuWindow" , "../../../bin/plugins/PhysicalItems/qml/menuWindow/" +        m_typeName + "MenuWindow.qml"},
        {"properties" , "../../../bin/plugins/PhysicalItems/qml/properties/" +        m_typeName + "Properties.qml"},
        {"sceneLoader" ,      "../../../bin/plugins/PhysicalItems/qml/sceneLoader/" + m_typeName + "SceneLoader.qml"     },
        {"addItemMenuButton", "../../../bin/plugins/PhysicalItems/qml/menuButton/" +  m_typeName + "AddItemMenuButton.qml"},
        {"addItemMenuWindow", "../../../bin/plugins/PhysicalItems/qml/menuWindow/" +  m_typeName + "AddItemMenuWindow.qml"}}},
        {"tools", {
        {"", ""}}}
    };
};

template<class T>
void PhysicalItemsPlugin::createCreator(QString typeName)
{
    auto f = new SceneItemsFactory([]()->SceneItem*{ return new T();});
    f->setObjectName(typeName);
//    f->setComponentsMap(m);
    SceneItemsFactory * factory = f;
    creators.push_back(factory);
}

template<class T>
void PhysicalItemsPlugin::createController(QString typeName)
{
    auto cf = new ControllerFactory([]()->BaseMoveController*{ return new T();});
    cf->setObjectName(typeName);
    controllers.push_back(cf);
}


#endif // SCENEITEMS_PLUGIN_H
