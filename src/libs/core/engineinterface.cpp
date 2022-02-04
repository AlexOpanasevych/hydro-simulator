#include "basemsg.h"
#include "engineinterface.h"
#include "messagedispatcher.h"

EngineInterface::EngineInterface(): fromEngine(200), toEngine(200)
{

    answerDispatcher = new MessageAnswerDispatcher;
}

EngineInterface::~EngineInterface()
{
    delete answerDispatcher;
}

void EngineInterface::proceed() {
    while (true) {
        auto bA = fromEngine.pop();
        if (bA.size() < 4) break; //no message to proceed
        QDataStream s(bA);
        //HeaderAnswer msg = getHeaderAnswer(&s);
        Header h;
        s >> h;
        //proceed answer
        int pos = s.device()->pos();
//        int type = -1;
        if (h.isAnswer) {
//            if (h.type < 0) qDebug() << h.type;
            answerDispatcher->proceedMsgAnswer(h, s);
        }
        else {

            auto search = msgFromEngineHandlers.find(h.type);
            if(search != msgFromEngineHandlers.end()){
                search->second(s, h.itemId);
            }

        }
            s.device()->seek(pos);
            runConnections(h.type, s);
        //proceed stream msg
        //streamDispatcher->proceedMsg(&h, s);
    }
}

//TorpedoSampleData EngineInterface::getLastDataTorpedo(int piDescriptor)
//{
//    return m_engine->getPiSamples()->getLastSample<TorpedoSampleData>(piDescriptor, 0);//, m_engine->getControllerSamples()->getLastSample<TorpedoControllerSampleData>(piDescriptor)};
//}

//void EngineInterface::getLastDataShip(int piDescriptor, ShipSampleData * shData, ShipControllerSampleData * scData)
//{
//    ShipSampleData shOData = m_engine->getPiSamples()->getLastSample<ShipSampleData>(piDescriptor, 0);
//    ShipControllerSampleData scOData = m_engine->getControllerSamples()->getLastSample<ShipControllerSampleData>(piDescriptor, 0);
//    shData = &shOData;
//    scData = &scOData;
//    return;
//}

//SubmarineSampleData EngineInterface::getLastDataSubmarine(int piDescriptor)
//{
//    return m_engine->getPiSamples()->getLastSample<SubmarineSampleData>(piDescriptor);
//}

void EngineInterface::_pushAnswerEngineSide(const QByteArray & msg, Header * header)
{
    QByteArray data;
    QDataStream s(&data, QIODevice::ReadWrite);
    s << *header;
    data.append(msg);
    fromEngine.push(data);
}










