#ifndef MESSAGEANSWERDISPATCHER_H
#define MESSAGEANSWERDISPATCHER_H

#include "basemsg.h"
#include <exception>
#include <iostream>
#include <QDebug>

class MessageAnswerDispatcher;

class MsgAnswerHandler
{
public:
    MsgAnswerHandler(MessageAnswerDispatcher * disp, int m_uid) : answerDispatcher(disp), m_uid(m_uid) {}

    template<class TAns>
    //!
    //! \brief Set callback when answer is achieved
    //! \param callback
    //!
    void onCompleted(std::function<void (TAns)> callback);

private:
    MessageAnswerDispatcher * answerDispatcher;
    int m_uid;
};


class MessageAnswerDispatcher
{

    friend class MsgAnswerHandler;
    std::map<int, std::function<void(const Header & h, QDataStream &s)>> m_answerHandlers;
public:

    MessageAnswerDispatcher();

    //!
    //! \brief Proceeds answer from engine and executes callback
    //! \param h
    //! \param s
    //!
    void proceedMsgAnswer(const Header & h, QDataStream &s){
        auto search = m_answerHandlers.find(h.uid); ///?????????????????
        if (search != m_answerHandlers.end()) {
            search->second(h, s);
            m_answerHandlers.erase(search->first);
        }
    }

};

template<class TAns>
void MsgAnswerHandler::onCompleted(std::function<void (TAns)> callback)
{
    //static_assert (std::is_base_of<BaseMsg, TAns>(), "Not derived from BaseMsg");

    auto lambda = [callback](const Header& h, QDataStream &s){
        TAns answ;
        try {
            answ.deserialize(&s);
            if (h.type != answ.getType()) {
                throw std::exception();
            }
            callback(answ);
        } catch (const std::exception & e) {
            std::cerr << "types are not the same! " << h.type << " " << answ.getType() << std::endl;
        }
    };

    answerDispatcher->m_answerHandlers.insert({m_uid, lambda});

}
#endif // MESSAGEANSWERDISPATCHER_H
