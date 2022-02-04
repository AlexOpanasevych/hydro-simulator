#ifndef SUBMARINEITEM_H
#define SUBMARINEITEM_H

#include <sceneitems/cpp/ksceneitem.h>
#include <libs/klibcore/kmacro.h>

class SubmarineItem : public KSceneItem
{
    Q_OBJECT
    K_QML_TYPE(SubmarineItem)
public:
    SubmarineItem(QObject *parent = nullptr);
};

#endif // SUBMARINEITEM_H
