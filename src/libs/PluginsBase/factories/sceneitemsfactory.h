#ifndef SCENEITEMSFACTORY_H
#define SCENEITEMSFACTORY_H

#include <QObject>
#include <pluginsbase_global.h>
#include <libs/klibcorelite/kmacro.h>
#include <base/sceneitem.h>

class PLUGINSBASE_EXPORT SceneItemsFactory : public QObject
{
    Q_OBJECT
//    K_AUTO_PROPERTY(QString, objectName, objectName, setObjectName, objectNameChanged, "")

public:
    SceneItemsFactory(std::function<SceneItem*()> cr) : m_creator(cr) {}

    SceneItem *create(QVariantMap params){ auto item = m_creator(); setParams(item, params); return item; }
    QString creatorDialogUrl(){return dialogUrl;}
//    QQmlComponent* component(QString type){if (this->componentsMap.contains(type)) { return this->componentsMap[type]; } else { return nullptr;} }
    std::function<bool(QObject*)> proxyFilter(){return m_filter;}

    void setDialogUrl(QString url) { this->dialogUrl = url; }
//    void setComponentsMap(QMap<QString, QQmlComponent*> m) { this->componentsMap = m; }
    void setProxyFilter(std::function<bool (QObject *)> filter) { this->m_filter = filter; }
private:
    QString dialogUrl = "";
    std::function<bool (QObject *)> m_filter = nullptr;
//    QMap<QString, QQmlComponent*> componentsMap;

    void setParams(QObject *obj, QVariantMap params)
    {
        QMapIterator iter(params);
        QMap<QString, int> properties;

        for (int i = 0, size = obj->metaObject()->propertyCount(); i < size; i++)
        {
            auto p = obj->metaObject()->property(i);
            properties.insert(p.name(), i);
        }

        while (iter.hasNext())
        {
            auto p = iter.next();
            if (properties.contains(p.key()))
            {
                obj->metaObject()->property(properties[p.key()]).write(obj, p.value());
            }
        }
    }

private:
    std::function<SceneItem*()> m_creator = nullptr;
};

#endif // SCENEITEMSFACTORY_H
