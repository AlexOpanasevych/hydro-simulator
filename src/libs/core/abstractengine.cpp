#include "abstractengine.h"

//std::mutex AbstractEngine::m;

AbstractEngine::AbstractEngine()
{
    m_interface = new EngineInterface;
    m_interface->m_engine = this;
    piSamples = new SampleDataContainer;
    //waveSamples = new WaveSampleDataContainer;
    messageDispatcher = new MessageDispatcher;
    //controllerSamples = new SamplaDataContainer;

    doMath = false;
}

AbstractEngine::~AbstractEngine()
{
    delete m_interface;
    delete piSamples;
    delete messageDispatcher;

//    delete controllerSamples;
//    for(auto p : m_piControllers){
//        delete p.second;
//    }
}

SampleDataContainer *AbstractEngine::getPiSamples() const
{
//    std::lock_guard<std::mutex> l(m);
    return piSamples;
}

//WaveSampleDataContainer *AbstractEngine::getWaveSamples() const
//{
//    return waveSamples;
//}

EngineInterface *AbstractEngine::getInterface() const
{
    return m_interface;
}

void AbstractEngine::proceed(int uSecond, int dt)
{
    Q_UNUSED(dt)
    //take msgs from interface and proceed them
    while (true) {
        QByteArray serMsg = m_interface->toEngine.pop();
        if (serMsg.isEmpty()) break;
        QDataStream stream(&serMsg, QIODevice::ReadOnly);
        Header msgHeader;
        stream >> msgHeader;

        QByteArray answ;
        if (msgHeader.itemId == -1) { //msg for controller
            answ = messageDispatcher->proceedMsg(&msgHeader, stream);

        } else { //global msg
            answ = proceedItemMsg(msgHeader, stream);
        }
        if (answ.size()) {
            m_interface->_pushAnswerEngineSide(answ, &msgHeader);
        }
    }
    if(doMath){
        //proceed physical items
        prevTime_us = uSecond;
        for (auto c : m_piControllers) {
            c.second->proceed(m_engineTime + dt, dt);
        }

        for (auto c : m_piControllers) {
            c.second->proceedAfter(m_engineTime + dt, dt);
        }
        //proceed wave items
        //for (auto c : m_waveController) {
        //    c->proceed(uSecond);
        //}
        if(!(m_engineTime % 1000000))
            for(auto && fn : oneSecondCallbacks)
                fn();

        m_engineTime += dt; // uSecond
    }


}

void AbstractEngine::insertController(int id, AbstractPIController * c)
{
    m_piControllers.insert({id, c});
}

bool AbstractEngine::removeController(int id)
{
    AbstractPIController * p = m_piControllers[id];
    delete p;
    m_piControllers.erase(id);
    return true;
}

QByteArray AbstractEngine::proceedItemMsg(Header header, QDataStream &s)
{
    for (auto c : m_piControllers) {
        if (c.second->getPiId() == header.itemId) {
            return c.second->proceedMsg(&header, s);
        }
    }
    //todo: do same for wavecontrollers
    return nullptr;
}

std::map<int, AbstractPIController *> & AbstractEngine::getPiControllers()
{
    return m_piControllers;
}

//unsigned long long AbstractEngine::engineTime() const
//{
//    return m_engineTime;
//}

//SamplaDataContainer *AbstractEngine::getControllerSamples() const
//{
//    return controllerSamples;
//}
