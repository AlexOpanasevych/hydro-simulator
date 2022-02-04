#ifndef TORPEDOITEM_H
#define TORPEDOITEM_H

#include <sceneitems/cpp/ksceneitem.h>
#include <libs/klibcore/kmacro.h>

class TorpedoItem : public KSceneItem
{
    Q_OBJECT
    K_QML_TYPE(TorpedoItem)
public:
    TorpedoItem(QObject *parent = nullptr);
};

#endif // TORPEDOITEM_H
