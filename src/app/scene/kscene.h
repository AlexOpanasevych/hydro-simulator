#ifndef KSCENE_H
#define KSCENE_H

#include <QObject>
#include <libs/klibcorelite/kmacro.h>
#include <libs/klibcorelite/utility/kobservablelist.h>
#include <QPointF>
#include <QTimer>
#include <libs/core/engineinterface.h>
#include <app/scene/ksceneplayer.h>
#include <app/tools/flexibleproxymodel.h>
#include <app/scene/ksceneplayer.h>
#include <app/tools/flexibleproxymodel.h>
#include <app/tools/objectservice.h>
#include <base/scenebase.h>

class SceneItem;
class ControllerFactory;
class SceneItemsFactory;
class TrajectoryParser;
class ComponentsProvider;

//!
//! \brief The KScene class - central logical core of visual layer
//!
class KScene : public SceneBase
{
    Q_OBJECT
    K_QML_TYPE(KScene)
    K_AUTO_PROPERTY(bool, devMode, devMode, setdevMode, devModeChanged, false)
    K_AUTO_PROPERTY(bool, adminMode, adminMode, setAdminMode, adminModeChanged, true)
    K_AUTO_PROPERTY(QQmlEngine *, engine, engine, setEngine, engineChanged, nullptr)
    K_CONST_PROPERTY(KScenePlayer *, scenePlayer, new KScenePlayer(this))
    K_CONST_PROPERTY(FlexibleProxyModel *, proxy, new FlexibleProxyModel(this))
    K_CONST_PROPERTY(PhysicalObjectsService *, piService, new PhysicalObjectsService(this))
public:
    explicit KScene(QObject *parent = nullptr);
    virtual void addFactory(QList<SceneItemsFactory*> factories) override;
    virtual void addComponents(QMap<QString, QMap<QString, QMap<QString, QString>>> sceneComponents) override;

public slots:
    virtual SceneItem *addItem(QPointF pos, QString objectName) override;
    virtual SceneItem *addItem(double x, double y, QString type) override;
    virtual void updateItems() override;
    virtual void removeAllItems(QString type = "") override;
    virtual void setEngineInterface(EngineInterface * ei) override;
    virtual void removeItem(SceneItem* item) override;

    virtual QQmlComponent * getComponent(SceneItem *object, QString type) override;
    virtual QQmlComponent * getComponent(QString category, QString objectName, QString type) override;
    virtual QVariantList getComponents(QString type) override;
    void activateFilter(QString type);
    void load();
    void save();
signals:

private:
    //!
    //! \brief Creates component from given objectName and type
    //! \param objectName
    //! \param type
    //! \return
    //!
    QQmlComponent *createComponent(QString category, QString objectName, QString type);

    EngineInterface *ei = nullptr;
    QTimer * timer = nullptr;
    int m_sceneId = -1;

    std::function<bool(QObject*)> m_submarineFilter = nullptr;
    std::function<bool(QObject*)> m_torpedoFilter = nullptr;
    std::function<bool(QObject*)> m_shipFilter = nullptr;

    ComponentsProvider *m_cprovider = nullptr;
    //TrajectoryParser *parser = nullptr;
};

#endif // KSCENE_H
