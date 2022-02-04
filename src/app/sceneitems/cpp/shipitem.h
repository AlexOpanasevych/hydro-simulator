#ifndef SHIPITEM_H
#define SHIPITEM_H

#include <sceneitems/cpp/ksceneitem.h>
#include <libs/klibcore/kmacro.h>

class ShipItem : public KSceneItem
{
    Q_OBJECT
    K_QML_TYPE(ShipItem)
public:
    ShipItem(QObject *parent = nullptr);
};

#endif // SHIPITEM_H
