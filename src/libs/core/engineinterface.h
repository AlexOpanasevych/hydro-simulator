#ifndef ENGINEINTERFACE_H
#define ENGINEINTERFACE_H

#include "core_global.h"
#include <kcirclebuffer.h>
#include <abstractengine.h>
#include <messageanswerdispatcher.h>
#include <QDataStream>
#include <map>
#include <utility/VariantContainer.h>


struct BaseMsg;
class MessageDispatcher;
class AbstractEngine;
class SampleDataGetter;

class CORE_LIBRARY_EXPORT MessageConnector {
    std::function<void(QDataStream &)> callbackOnMsg;
    int messageType;
public:
    //!
    //! \brief Get message type
    //! \return message type of \a this connector
    //!
    int getMessageType() {return messageType;}

    template <typename TMsg, typename Callable>

    //!
    //! \brief Connects Callable \p m to particular message from engine
    //! \param m
    //!
    void connectToMsg(Callable m)
    {
        messageType = TMsg().getType();
        callbackOnMsg = [m](QDataStream & s){
            TMsg msg;
            msg.deserialize(&s);
            m(msg);
        };
    }

    //!
    //! \brief Calls callback when message is triggered without using \a callbackOnMsg function
    //! \param s
    //!
    void operator()(QDataStream &s) {
        callbackOnMsg(s);
    }
};


class CORE_LIBRARY_EXPORT EngineInterface
{
    friend class AbstractEngine;
    friend MsgAnswerHandler;
    friend class SampleDataGetter;
public:
    EngineInterface();
    ~EngineInterface();

    //!
    //! \brief Proceeds all incoming messages
    //!
    void proceed();

    template<class TMsg>
    //!
    //! \brief Sends message to internal engine, and bind callback
    //! \param msg
    //! \param itemId
    //! \return MsgAnswerHandler object
    //!
    MsgAnswerHandler sendToEngine(TMsg msg, int itemId = -1);


    template<class TMsg>
    //!
    //! \brief Install stream msg that comes from engine
    //!
    void installStreamMsg(std::function<void(TMsg, int)>);
//    TorpedoSampleData getLastDataTorpedo(int piDescriptor);
//    void getLastDataShip(int piDescriptor, ShipSampleData * shData, ShipControllerSampleData * scData);
//    SubmarineSampleData getLastDataSubmarine(int piDescriptor);
    //Wave * getLastData(int type, int piDescriptor);
    template<class TMsg, class ... TMsgs>
    //!
    //! \brief Sends message from engine
    //! \param msg
    //! \param msgs
    //! \param itemId
    //!
    void sendFromEngine(TMsg msg, TMsgs ... msgs, int itemId = -1);

    //!
    //! \brief Sends answer from engine side
    //! \param msg
    //! \param header
    //!
    void _pushAnswerEngineSide(const QByteArray &msg, Header *header);

    template<typename T>
    //!
    //! \brief Get last data from container
    //! \param piId
    //! \return
    //!
    T * getLastData(int piId)
    {
        return this->m_engine->getPiSamples()->template getLastSample<T>(piId);
    }

    template<typename TMsg, typename Callable>
    //!
    //! \brief Adds connection to message and binds callback
    //! \param f
    //!
    void addConnection(Callable f) {

        int type = TMsg().getType();

        MessageConnector mConn;
        mConn.connectToMsg<TMsg>(f);

        msgConnections[type].push_back(mConn);

    }

    //!
    //! \brief Run connections that are binded to paricular \a msg
    //! \param type
    //! \param s
    //!
    void runConnections(int type, QDataStream &s) {
        if(msgConnections.find(type) != msgConnections.end()) {
            int pos = s.device()->pos();
            for(auto mConn : msgConnections[type]) {
                mConn(s);
                s.device()->seek(pos);
            }
        }
    }

protected:

private:
    KCircleBuffer<QByteArray> fromEngine;
    KCircleBuffer<QByteArray> toEngine;

    int nextMsgId=0;
    int nextMsgIdEngineSide=0;


    MessageAnswerDispatcher * answerDispatcher;
    AbstractEngine *m_engine;

    std::map<int, std::function<int(QDataStream &s, int)>> msgFromEngineHandlers;
    //
    std::map<int, std::vector<MessageConnector>> msgConnections;

};



template<class TMsg, class ... TMsgs>
void EngineInterface::sendFromEngine(TMsg msg, TMsgs ... msgs, int itemId)
{
    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    //write header
    Header header;
    header.uid = this->nextMsgIdEngineSide++;
//    qDebug() << TMsg().getType();
    header.type = TMsg().getType();
    header.itemId = itemId;
    header.isAnswer = false;
    s << header;
    //write msg
    msg.serialize(&s);
    fromEngine.push(data);
    if constexpr (sizeof...(msgs) > 0) {
        sendFromEngine(msgs..., itemId);
    }
}

template <class TMsg>
MsgAnswerHandler EngineInterface::sendToEngine(TMsg msg, int itemId)
{
    QByteArray data;
    QDataStream s(&data, QIODevice::WriteOnly);
    //write header
    Header header;
    header.uid = this->nextMsgId++;
    header.type = msg.getType();
    header.itemId = itemId;
    header.isAnswer = false;
    s << header;
    //write msg
    msg.serialize(&s);
    toEngine.push(data);
    MsgAnswerHandler handler(this->answerDispatcher, header.uid);
    return handler;
}

template<class TMsg>
void EngineInterface::installStreamMsg(std::function<void(TMsg, int)> lambda){

    auto type = TMsg().getType();

    auto funcTobeCalled = [lambda](QDataStream &s, int itemId) {
        //call handler
        TMsg msg;
        msg.deserialize(&s);
        lambda(msg, itemId);
        return msg.getType();
    };
    msgFromEngineHandlers.insert({type, funcTobeCalled});
}

class CORE_LIBRARY_EXPORT SampleDataGetter
{
public:
    SampleDataGetter(EngineInterface* ei):m_ei(ei){};
    template<class TSD> TSD * getSD(int id){return m_ei->getLastData<TSD>(id);}

private:
    EngineInterface *m_ei;
};

#endif // ENGINEINTERFACE_H
