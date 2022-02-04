#ifndef CHANGEANGLEMSG_H
#define CHANGEANGLEMSG_H

#include <libs/core/basemsg.h>
#include <vessel_global.h>

struct VESSEL_EXPORT ChangeAngleMsg : BaseMsg {
    private: inline static int m_type = 5;\
    public: int getType() const override {return m_type;}\
    static bool checkType(int type) {return type == m_type;}\
    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
    double angle;
    void serialize(QDataStream *) const override;
    void deserialize(QDataStream *) override;
};

struct VESSEL_EXPORT ChangeAngleMsgAns : BaseMsg {
    private: inline static int m_type = 6;\
    public: int getType() const override {return m_type;}\
    static bool checkType(int type) {return type == m_type;}\
    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
    bool changed;
    void serialize(QDataStream *) const override;
    void deserialize(QDataStream *) override;
};

#endif // CHANGEANGLEMSG_H
