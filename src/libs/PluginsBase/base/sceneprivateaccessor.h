#ifndef SCENEPRIVATEACCESSOR_H
#define SCENEPRIVATEACCESSOR_H

#include <QObject>
#include <QQmlComponent>
#include <pluginsbase_global.h>
#include <kmacro.h>
#include <utility/kobservablelist.h>
#include <base/sceneitem.h>
#include <QImage>
#include <factories/sceneitemsfactory.h>

class SceneBase;
/**
 * @brief The ScenePrivateAccessor class used only in saverpovider for getting access to items and conntrollers layer
 */
class PLUGINSBASE_EXPORT ScenePrivateAccessor : public QObject
{
    Q_OBJECT
public:
    explicit ScenePrivateAccessor(QObject *parent = nullptr);
    /**
     * @brief m_sceneItemsList
     */
    KObservableList<SceneItem> *m_sceneItemsList = nullptr;
    /**
     * @brief m_createdControllers
     */
    QHash<QString, QVariantMap> *m_createdControllers = nullptr;
    /**
     * @brief m_factoriesList
     */
    KObservableList<SceneItemsFactory> *m_factoriesList = nullptr;
    /**
     * @brief m_factoriesHash
     */
    QHash<QString, SceneItemsFactory*> *m_factoriesHash = nullptr;
    /**
     * @brief m_scene
     */
    SceneBase *m_scene = nullptr;
public slots:
    /**
      * @brief setScene
      * @param scene
      */
    void setScene(SceneBase *scene);
private:
    static inline const int ScenePrivateAccessorQMLRegistration = qmlRegisterType<ScenePrivateAccessor>("PluginsBase", 1, 0, "ScenePrivateAccessor");
};

#endif // SCENEPRIVATEACCESSOR_H
