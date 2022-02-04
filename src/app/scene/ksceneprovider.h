#ifndef KSCENEPROVIDER_H
#define KSCENEPROVIDER_H

#include <QPointF>
#include <QObject>
#include <libs/klibcorelite/kmacro.h>
#include <QQmlEngine>
class KScene;
class KSceneProvider : public QObject
{
    Q_OBJECT
    K_SINGLETON(KSceneProvider)
    K_QML_SINGLETON(KSceneProvider)
public:
    explicit KSceneProvider(QObject *parent = nullptr);

public slots:
    KScene *getScene(QString sceneID, QPointF size);
    KScene *getScene(QString sceneID, double width = 0, double height = 0);

signals:

private:
    QMap<QString, KScene *> m_scenes;
};

#endif // KSCENEPROVIDER_H
