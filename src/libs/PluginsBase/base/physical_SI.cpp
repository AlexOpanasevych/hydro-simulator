#include "physical_SI.h"

#include <base/global_messages/createpimsg.h>

PhysicalItem::PhysicalItem(QObject *parent) : SceneItem(parent)
{
    setType("PhisycalItem");
}

void PhysicalItem::move(QPointF pos)
{
    setX(pos.x());
    setY(pos.y());
    emit coordsUpdated(pos);
}

void PhysicalItem::proceed(SampleDataGetter & getter)
{
    Q_UNUSED(getter)
}

void PhysicalItem::init(EngineInterface * ei)
{
//    if(m_id) {
        CreatePIMsg msg;
        msg.position[0] = m_x;
        msg.position[1] = m_y;
        msg.position[2] = m_z;
        msg.velocity = m_velocity;
        msg.angle = m_rotation;
        msg.typeOfObject = type();
        ei->sendToEngine(msg).onCompleted<CreatePIMsgAns>([this](const CreatePIMsgAns & ans) {
            setId(ans.createdId);
        });
}
