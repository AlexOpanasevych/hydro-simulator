#ifndef CREATESHIPMSG_H
#define CREATESHIPMSG_H

#include <libs/core/basemsg.h>
#include <vessel_global.h>

//class TORPEDOENGINE_LIBRARY_EXPORT SetShipPathMsg : public BaseMsg
//{
//    private: inline static int m_type = 13;\
//    public: int getType() const override {return m_type;}\
//    static bool checkType(int type) {return type == m_type;}\
//    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
//    std::vector<std::array<double, 3>> path;
//    std::vector<std::array<double, 3>> torpedoEjectPoints;
//    std::vector<double> angle;
//    std::vector<double> velocity;
//public:
//    SetShipPathMsg();
//    void serialize(QDataStream *s) const override;
//    void deserialize(QDataStream *s) override;
//};

struct VESSEL_EXPORT SetShipPathMsg : public BaseMsg {
private: inline static int m_type = 13;\
    public: int getType() const override {return m_type;}\
    static bool checkType(int type) {return type == m_type;}\
    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
    QString cmds;
    void serialize(QDataStream *s) const override;
    void deserialize(QDataStream *s) override;
};

struct VESSEL_EXPORT SetShipPathMsgAns : public BaseMsg
{
    private: inline static int m_type = 14;\
    public: int getType() const override {return m_type;}\
    static bool checkType(int type) {return type == m_type;}\
    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
    bool success;
public:
    void serialize(QDataStream *s) const override;
    void deserialize(QDataStream *s) override;
};

#endif // CREATESHIPMSG_H
