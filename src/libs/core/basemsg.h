#ifndef BASEMSG_H
#define BASEMSG_H

#include <QByteArray>
#include <map>
#include <functional>
#include <QDataStream>
#include <QDebug>
#include <set>
#include "core_global.h"

//!
//! \brief The BaseMsg struct
//!
struct CORE_LIBRARY_EXPORT BaseMsg
{
    BaseMsg();
    BaseMsg(const BaseMsg&);
    virtual ~BaseMsg();

    //!
    //! \brief Get type of message
    //! \return  \a this message type
    //!
    virtual int getType() const = 0;

    //!
    //! \brief Serializes \a this message in binary data
    //! \param s
    //!
    virtual void serialize(QDataStream *s) const = 0;

    //!
    //! \brief Deserializes \a this message from binary data
    //! \param s
    //!
    virtual void deserialize(QDataStream *s) = 0;

    //for debug
    inline static int instanceCount = 0;
};

struct CORE_LIBRARY_EXPORT Header {
    Header(int type_ = -1):type(type_){}
    bool isAnswer;
    int type;
    int uid; // if answer - put there request msg id, otherwise put msg id
    int itemId;
};

//!
//! \brief Serializes header in binary data
//! \param out
//! \param obj
//! \return reference to \p out stream
//!
QDataStream CORE_LIBRARY_EXPORT &operator<<(QDataStream &out, const Header& obj);

//!
//! \brief Deserializes header in binary data
//! \param in
//! \param obj
//! \return reference to \p out stream
//!
QDataStream CORE_LIBRARY_EXPORT &operator>>(QDataStream &in, Header &obj);


/* #define ENGINE_MSG(Type) \
     private: inline static int m_type = MessageFactory::instance()->registerMsg<Type>(#Type);\
     public: int getType() const override {return m_type;}\
     static bool checkType(int type) {return type == m_type;}\
     static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
*/


struct CORE_LIBRARY_EXPORT EmptyMessage : BaseMsg
{
    private: inline static int m_type = 0;\
    public: int getType() const override {return m_type;}\
    static bool checkType(int type) {return type == m_type;}\
    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
public:

    void serialize(QDataStream *) const override{};
    void deserialize(QDataStream *) override{};

};


#endif // BASEMSG_H
