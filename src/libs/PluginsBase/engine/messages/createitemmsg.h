#ifndef CREATEITEMMSG_H
#define CREATEITEMMSG_H

#include <basemsg.h>
#include "../../pluginsbase_global.h"

struct PLUGINSBASE_EXPORT CreateItemMsg : public BaseMsg
{

private: inline static int m_type = 19;\
public: int getType() const override {return m_type;}\
static bool checkType(int type) {return type == m_type;}\
static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
    CreateItemMsg();

    std::array<double, 3> pos;
    int id;
    int controllerId;
//    std::vector<double> velocity;
    double velocity;
    double angle;
    std::string typeOfObject;
    std::string color;

    // BaseMsg interface
public:
    void serialize(QDataStream *s) const override;
    void deserialize(QDataStream *s) override;
};

struct PLUGINSBASE_EXPORT CreateItemMsgAns : public BaseMsg
{
private: inline static int m_type = 20;\
public: int getType() const override {return m_type;}\
static bool checkType(int type) {return type == m_type;}\
static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
    CreateItemMsgAns();
    std::array<double, 3> pos;
    int controllerId;
};

#endif // CREATEITEMMSG_H
