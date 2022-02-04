#ifndef CHANGEVELOCITYMSG_H
#define CHANGEVELOCITYMSG_H

#include <libs/core/basemsg.h>
#include <vessel_global.h>

struct VESSEL_EXPORT ChangeVelocityMsg : BaseMsg {
    private: inline static int m_type = 9;\
    public: int getType() const override {return m_type;}\
    static bool checkType(int type) {return type == m_type;}\
    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
    double vel_x;
    double vel_y;
    double vel_z;
    void serialize(QDataStream *) const override;
    void deserialize(QDataStream *) override;
};

struct VESSEL_EXPORT ChangeVelocityMsgAns : BaseMsg {
    private: inline static int m_type = 10;\
    public: int getType() const override {return m_type;}\
    static bool checkType(int type) {return type == m_type;}\
    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
    bool success;
    void serialize(QDataStream *) const override;
    void deserialize(QDataStream *) override;
};

#endif // CHANGEVELOCITYMSG_H
