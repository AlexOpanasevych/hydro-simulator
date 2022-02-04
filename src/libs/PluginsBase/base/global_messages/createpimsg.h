#ifndef CREATEPIMSG_H
#define CREATEPIMSG_H

#include <libs/core/basemsg.h>
#include <pluginsbase_global.h>

struct PLUGINSBASE_EXPORT CreatePIMsg : BaseMsg
{
    private: inline static int m_type = 1;\
    public: int getType() const override {return m_type;}\
    static bool checkType(int type) {return type == m_type;}\
    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}

    double position[3];
    double velocity;
    double angle;
    QString typeOfObject;
    void serialize(QDataStream *) const override;
    void deserialize(QDataStream *) override;
};

struct PLUGINSBASE_EXPORT CreatePIMsgAns : BaseMsg {
    private: inline static int m_type = 2;\
    public: int getType() const override {return m_type;}\
    static bool checkType(int type) {return type == m_type;}\
    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
    int createdId;
    double position[3];
    double velocity;
    double angle;
    QString typeOfObject;
    void serialize(QDataStream *) const override;
    void deserialize(QDataStream *) override;
};

#endif // CREATEPIMSG_H
