#include "torpedo.h"
#include <engine/sampledata/torpedocontrollersampledata.h>
#include <engine/sampledata/torpedosampledata.h>
#include <base/global_messages/createpimsg.h>
#include <engine/messages/settorpedoparams.h>
#include <cdebug.h>
#include <math/kmath.h>

Torpedo::Torpedo(QObject *parent) : PhysicalItem(parent)
{
    setType("Torpedo");
}

void Torpedo::proceed(SampleDataGetter &getter)
{
    auto lastData = getter.getSD<TorpedoSampleData>(this->id());
    auto lastDataC = getter.getSD<TorpedoControllerSampleData>(this->id());

    if(lastData != nullptr and lastDataC != nullptr) {
        move(QPointF(lastData->position[0], lastData->position[1]));
        trace()->updateCoords(QPointF(lastData->position[0], -lastData->position[1])/*, Trace::EXPAND_SEGMENT*/);
        //this->setX(lastData->position[0]);
        //this->setY(lastData->position[1]);
        this->setZ(lastData->position[2]);
        this->setRotation(KMath::radiansToDegrees(lastDataC->alpha));
        this->setVelocity(KMath::toKnots(lastData->velocity));
        this->setDead(lastData->destroyed);
        this->setCurrentDistance(lastData->currentDistance);
        this->setCourseAngle(KMath::radiansToDegrees(lastData->beta));
        this->setDistanceToGoal(lastData->distanceToGoal);
        this->setCurrentTime(lastData->onLife);
        this->setGoalFound(lastData->goalFound);
        if(!m_stages.isEmpty()) {
            if(m_stages.last() != lastDataC->stage) {
    //            trace()->updateCoords(QPointF(lastData->position[0], -lastData->position[1]), Trace::START_SEGMENT);
                qDebug(l) << lastDataC->stage;
                m_stages.push_back(lastDataC->stage);
                emit stagesChanged(m_stages);
                qDebug(s) << m_stages.last();
            }
    //        else
    //        {
    //            trace()->updateCoords(QPointF(lastData->position[0], -lastData->position[1]), Trace::EXPAND_SEGMENT);
    //        }
        }
        else {
            m_stages.push_back(lastDataC->stage);
            qDebug(l) << lastDataC->stage;
    //        trace()->updateCoords(QPointF(lastData->position[0], -lastData->position[1]), Trace::START_SEGMENT);
            emit stagesChanged(m_stages);
            qDebug(s) << m_stages.last();
        }


    }

}

QVariantMap Torpedo::serialize()
{
    QVariantMap m;
    m = {
        { "x" ,       x()},
        { "y",        y()},
        { "z",        z()},
        { "sceneId",  sceneId()},
        { "width",    width() },
        { "height",   height() },
        { "rotation", rotation()},
        { "firstRa",  m_firstRa},
        { "secondRa", m_secondRa},
        { "goalBoard", m_goalBoard},
        { "boardAngle", m_boardAngle},
        { "torpedoDivergency", m_torpedoDivergency},
        { "firstRotateDistance", m_firstRotateDistance},
        { "type", type()},
        { "objectName", objectName() }
    };
    return m;
}

void Torpedo::deserialize(QVariantMap map)
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
    setFirstRa(map.value("firstRa", 0).toInt());
    setSecondRa(map.value("secondRa", 0).toInt());
    setGoalBoard((Board)map.value("goalBoard", 0).toInt());
    setBoardAngle(map.value("boardAngle", 0).toInt());
    setFirstRotateDistance(map.value("firstRotateDistance", 250).toDouble());
    setTorpedoDivergency(map.value("torpedoDivergency", 0).toInt());
}

void Torpedo::init(EngineInterface *ei)
{
    CreatePIMsg msg;
    msg.position[0] = x();
    msg.position[1] = y();
    msg.position[2] = z();
    msg.velocity = KMath::fromKnots(40);
    msg.angle = rotation();
    msg.typeOfObject = type();
    ei->sendToEngine(msg).onCompleted<CreatePIMsgAns>([this, ei](const CreatePIMsgAns & ans) {
        setId(ans.createdId);
        SetTorpedoParams msg;
        msg.firstRA = m_firstRa;
        msg.secondRA = m_secondRa;
        msg.torpedoDivergency = m_torpedoDivergency;
        msg.rotateDistance = m_firstRotateDistance;
//        auto traj = item->property("trajectory");
        ei->sendToEngine(msg, id()).onCompleted<SetTorpedoParamsAns>([](const SetTorpedoParamsAns & ans) {
            qDebug(s) << "ok?" << ans.success;
        });
    });
}
