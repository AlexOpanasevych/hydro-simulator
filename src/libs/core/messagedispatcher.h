#ifndef MESSAGEDISPATCHER_H
#define MESSAGEDISPATCHER_H

#include <QDataStream>
#include <map>
#include "basemsg.h"
#include "core_global.h"
#include <utility/VariantContainer.h>

class CORE_LIBRARY_EXPORT MessageDispatcher
{
public:
    MessageDispatcher();
    template<class TMsg, class TClass, class TAns>
    //!
    //! \brief Registers handler to particular message
    //! \param obj
    //!
    void registerMsgHandler(TAns (TClass::*m)(TMsg), TClass *obj){
        //static_assert (std::is_base_of<BaseMsg, TMsg>(), "is not derived msg");
        //static_assert (std::is_base_of<BaseMsg, TMsg>(), "is not derived msgAns");
        int type = TMsg().getType();

        auto funcTobeCalled = [=](Header* h, QDataStream &s)->QByteArray {
            //call handler
            TMsg msg;
            msg.deserialize(&s);
            TAns answ = (obj->*m)(msg);
            if (EmptyMessage::checkType(answ.getType())) {
                return {};
            }
            //has answer
            h->type = answ.getType();
            h->isAnswer = true;
            QByteArray arr;
            QDataStream answerSerialize(&arr, QIODevice::ReadWrite);
            answ.serialize(&answerSerialize);
            return arr;
        };
        msgHandlers.insert({type, funcTobeCalled});
    }

    //!
    //! \brief Executes callback when it is found in registered callbacks
    //! \param h
    //! \param s
    //! \return QByteArray with answer
    //!
    QByteArray proceedMsg(Header* h, QDataStream &s){
        auto found = msgHandlers.find(h->type);
            if(found != msgHandlers.end()){
                return found->second(h, s);
            }
        return nullptr;
    }

private:
    std::map<int, std::function<QByteArray(Header*, QDataStream &s)>> msgHandlers;
};

#endif // MESSAGEDISPATCHER_H
