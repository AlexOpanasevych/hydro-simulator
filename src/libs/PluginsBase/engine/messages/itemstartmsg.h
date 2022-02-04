#ifndef ITEMSTARTMSG_H
#define ITEMSTARTMSG_H

#include <basemsg.h>
#include <pluginsbase_global.h>

struct PLUGINSBASE_EXPORT ItemStartMsg : public BaseMsg
{
private: inline static int m_type = 17;\
public: int getType() const override {return m_type;}\
static bool checkType(int type) {return type == m_type;}\
static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
    QString itemName;
    int id;
void serialize(QDataStream *) const override;
void deserialize(QDataStream *) override;
};

struct PLUGINSBASE_EXPORT ItemStopMsg : public BaseMsg
{
private: inline static int m_type = 18;\
public: int getType() const override {return m_type;}\
static bool checkType(int type) {return type == m_type;}\
static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
    int id;
    QString itemName;
void serialize(QDataStream *) const override;
void deserialize(QDataStream *) override;
};

#endif // ITEMSTARTMSG_H
