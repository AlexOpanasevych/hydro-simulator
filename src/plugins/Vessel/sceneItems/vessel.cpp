#include "vessel.h"
#include <engine/sampledata/vesselsampledata.h>
#include <engine/sampledata/vesselcontrollersampledata.h>
#include <QVariantMap>
#include <base/global_messages/createpimsg.h>
#include <engine/messages/setshippathmsg.h>
#include <cdebug.h>
#include <math/kmath.h>

//#define meterToKnots(meters) (meters * 0.00051444)

Vessel::Vessel(QObject *parent) : PhysicalItem(parent)
{
    setType("Vessel");
}

void Vessel::proceed(SampleDataGetter &getter)
{
    auto lastData = getter.getSD<VesselSampleData>(this->id());
    auto lastDataC = getter.getSD<VesselControllerSampleData>(this->id());
    if(lastData != nullptr and lastDataC != nullptr) {
        move(QPointF(lastData->position[0], lastData->position[1]));
//        trace()->updateCoords(QPointF(lastData->position[0], -lastData->position[1])/*, Trace::EXPAND_SEGMENT*/);
        this->setZ(lastData->position[2]);
        this->setRotation(lastDataC->angle);
        this->setVelocity(KMath::toKnots(lastData->velocity));
        this->setDead(lastData->destroyed);
        if(!m_stages.isEmpty()) {
            if(m_stages.last() != lastDataC->stage) {
    //            trace()->updateCoords(QPointF(lastData->position[0], -lastData->position[1]), Trace::START_SEGMENT);
                m_stages.push_back(lastDataC->stage);
                emit stagesChanged(m_stages);
                qDebug() << m_stages.last();
            }
        }
        else {
            m_stages.push_back(lastDataC->stage);
    //        trace()->updateCoords(QPointF(lastData->position[0], -lastData->position[1]), Trace::START_SEGMENT);
            emit stagesChanged(m_stages);
            qDebug() << m_stages.last();
        }

    }
}

void Vessel::addExtensionData(const QVariantMap &data)
{
    m_extensionData.insert(data);
}

void Vessel::removeExtensionData(const QString &datakey)
{
    m_extensionData.remove(datakey);
}

void Vessel::createTrajectory(QString initialParams)
{
    if (trajectory() != nullptr) return;

    auto traj = new Trajectory(this);
    traj->setX(x());
    traj->setY(-y());
    traj->setProperties(initialParams);
    setTrajectory(traj);
    //for test
    //trace()->updateCoords(QPointF(x(), -y()), Trace::START_SEGMENT);
}

void Vessel::removeTrajectory()
{
    m_trajectory->deleteLater();
    setTrajectory(nullptr);
}

QVariantMap Vessel::serialize()
{
    QVariantMap m;
    m = {
        { "x" , x()},
        { "y",  y()},
        { "z",  z()},
        { "sceneId", sceneId()},
        { "width", width() },
        { "height", height() },
        { "rotation", rotation()},
        { "type", type()},
        { "objectName", objectName() },
        { "extensionData", extensionData()}
    };

    if (trajectory() != nullptr)
        m.insert("trajectory_properties", trajectory()->properties());

    return m;
}

void Vessel::deserialize(QVariantMap map)
{
    setX(map.value("x", 0).toDouble());
    setY(map.value("y", 0).toDouble());
    setZ(map.value("z", 0).toDouble());
    setSceneId(map.value("sceneId", -1).toInt());
    setWidth(map.value("width", 0).toDouble());
    setHeight(map.value("height",  0).toDouble());
    setRotation(map.value("rotation",  0).toDouble());
    setType(map.value("type", "error").toString());
    setObjectName(map.value("objectName", "error").toString());
    setExtensionData(map.value("extensionData", QVariantMap()).toMap());

    auto traj = map.value("trajectory_properties", "").toString();
    if (traj != "")
    {
        createTrajectory(traj);
    }
}

void Vessel::init(EngineInterface * ei)
{
    CreatePIMsg msg;
    msg.position[0] = x();
    msg.position[1] = y();
    msg.position[2] = z();
    msg.velocity = KMath::fromKnots(20);
    msg.angle = rotation();
    msg.typeOfObject = type();
    ei->sendToEngine(msg).onCompleted<CreatePIMsgAns>([this, ei](const CreatePIMsgAns & ans) {
        setId(ans.createdId);
        SetShipPathMsg msg;
//        auto traj = item->property("trajectory");
        if (m_trajectory)
        {
            msg.cmds = m_trajectory->properties();
            qDebug(s) << "setpath" << type() << id();
            ei->sendToEngine(msg, id()).onCompleted<SetShipPathMsgAns>([](const SetShipPathMsgAns & ans) {
                qDebug(s) << "ok" << ans.success;
            });
        }
    });
}
