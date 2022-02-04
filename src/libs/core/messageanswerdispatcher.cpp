#include "messageanswerdispatcher.h"

#include <QDataStream>

MessageAnswerDispatcher::MessageAnswerDispatcher()
{

}



//void MessageAnswerDispatcher::proceedMsgAnswer(int type, int m_uid, QDataStream & s)
//{
//
//        BaseMsg * msg = MessageFactory::createMsg(type);
//        msg->deserialize(&s);
//        auto search = m_answerHandlers.find(m_uid); ///?????????????????
//        if (search != m_answerHandlers.end()) {
//            search->second(msg);
//            m_answerHandlers.erase(search->first);
//        }
//        delete msg;
//}
//
