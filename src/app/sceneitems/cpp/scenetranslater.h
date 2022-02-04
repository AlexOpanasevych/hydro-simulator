#ifndef SCENETRANSLATER_H
#define SCENETRANSLATER_H

#include <QObject>
#include <libs/klibcore/kmacro.h>
#include <QPointF>
//#include <sources/sceneitems/cpp/ksceneitem.h>
class KScene;
class KSceneItem;
class SceneTranslater : public QObject
{
    Q_OBJECT
    K_QML_TYPE(SceneTranslater)
    K_READONLY_PROPERTY(double, sceneX, sceneX, setSceneX, sceneXChanged, 0)
    K_READONLY_PROPERTY(double, sceneY, sceneY, setSceneY, sceneYChanged, 0)
    K_AUTO_PROPERTY(KSceneItem*, sceneItem, sceneItem, setSceneItem, sceneItemChanged, nullptr)
    K_AUTO_PROPERTY(KScene *, scene, scene, setScene, sceneChanged, nullptr)
public:
    explicit SceneTranslater(QObject *parent = nullptr);

public slots:
    QPointF toSceneCoords(double x, double y);
    QPointF fromSceneCoords(double x, double y);

signals:

};

#endif // SCENETRANSLATER_H
