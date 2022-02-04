#ifndef SCENEBASE_H
#define SCENEBASE_H

#include <QObject>
#include <QQmlComponent>
#include <libs/klibcorelite/kmacro.h>
#include <libs/klibcorelite/utility/kobservablelist.h>
#include <base/sceneitem.h>
#include <factories/sceneitemsfactory.cpp>
#include <libs/core/engineinterface.h>

typedef QMap<QString, QQmlComponent*> QmlComponents;
typedef QMap<QString, QString> PathComponents;
typedef QMap<QString, QmlComponents> PluginQmlComponents;
typedef QMap<QString, PathComponents> PluginPathComponents;
typedef QMap<QString, PluginQmlComponents> QQmlComponentsContainer;
typedef QMap<QString, PluginPathComponents> PathComponentsContainer;

class PLUGINSBASE_EXPORT SceneBase : public QObject
{
    Q_OBJECT
    K_AUTO_PROPERTY(double, sceneWidth, sceneWidth, setSceneWidth, sceneWidthChanged, 5000)
    K_AUTO_PROPERTY(double, sceneHeight, sceneHeight, setSceneHeight, sceneHeightChanged, 5000)
    K_PROTECTED_PROPERTY(KObservableModel *, sceneItems, sceneItems, setSceneItems, sceneItemsChanged, nullptr)
    K_PROTECTED_PROPERTY(QStringList, creatableItems, creatableItems, setCreatableItems, creatableItemsChanged, QStringList())
    friend class ScenePrivateAccessor;
public:
    explicit SceneBase(QObject *parent = nullptr);
    virtual void addFactory(QList<SceneItemsFactory*> factories) = 0;
    virtual void addComponents(QMap<QString, QMap<QString, QMap<QString, QString>>> sceneComponents) = 0;
public slots:
    virtual SceneItem *addItem(QPointF pos, QString objectName) = 0;
    virtual SceneItem *addItem(double x, double y, QString type) = 0;
    virtual void updateItems() = 0;
    virtual void removeAllItems(QString type = "") = 0;
    virtual void setEngineInterface(EngineInterface * ei) = 0;
    virtual void removeItem(SceneItem* item) = 0;
    virtual QQmlComponent * getComponent(SceneItem *object, QString type) = 0;
    virtual QQmlComponent * getComponent(QString category, QString objectName, QString type) = 0;
    virtual QVariantList getComponents(QString type) = 0;
protected:
    KObservableList<SceneItem> sceneItemsList;

    KObservableList<SceneItemsFactory> factoriesList;
    QHash<QString, SceneItemsFactory*> factoriesHash;
    QHash<QString, QVariantMap> createdControllers;

    QQmlComponentsContainer createdComponents;
    PathComponentsContainer allComponents;
private:
    static inline const int SceneBaseQMLRegistration = qmlRegisterInterface<SceneBase>("PluginsBase", 1);
};

#endif // SCENEBASE_H
