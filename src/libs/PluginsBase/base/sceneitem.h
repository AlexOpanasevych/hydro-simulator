#ifndef SCENEITEM_H
#define SCENEITEM_H

#include <QObject>
#include <QPointF>
#include <pluginsbase_global.h>
#include <libs/klibcorelite/kmacro.h>

class EngineInterface;
class SampleDataGetter;
class PLUGINSBASE_EXPORT SceneItem : public QObject
{
    Q_OBJECT
    K_AUTO_PROPERTY(QString, type, type, setType, typeChanged, "SceneItem")
    K_AUTO_PROPERTY(bool, interactive, interactive, setInteractive, interactiveChanged, false)
    K_AUTO_PROPERTY(int, sceneId, sceneId, setSceneId, sceneIdChanged, -1)
    K_PROTECTED_PROPERTY(QVariantMap, extensionData, extensionData, setExtensionData, extensionDataChanged, QVariantMap())
public:
    explicit SceneItem(QObject *parent = nullptr);
    virtual QVariantMap serialize() { return {}; }
    virtual void deserialize(QVariantMap map) { Q_UNUSED(map) }
    virtual void proceed(SampleDataGetter & getter) { Q_UNUSED(getter)}
    virtual void addExtensionData(const QVariantMap &data) { Q_UNUSED(data)}
    virtual void removeExtensionData(const QString &datakey) { Q_UNUSED(datakey)}
    virtual void init(EngineInterface *);
signals:
    void pressed();
private:
    static inline const int SceneItemQMLRegistration = qmlRegisterType<SceneItem>("PluginsBase", 1, 0, "SceneItem");
};

#endif // SCENEITEM_H
