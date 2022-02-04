#ifndef REMOVEITEMMESSAGE_H
#define REMOVEITEMMESSAGE_H

#include <libs/core/basemsg.h>
#include <pluginsbase_global.h>

struct PLUGINSBASE_EXPORT RemoveItemMessage : BaseMsg
{
    private: inline static int m_type = 3;\
    public: int getType() const override {return m_type;}\
    static bool checkType(int type) {return type == m_type;}\
    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
    std::vector<int> ids;
    //int removeId;
    RemoveItemMessage();
    void serialize(QDataStream *s) const override;
    void deserialize(QDataStream *s) override;
};

struct PLUGINSBASE_EXPORT RemoveItemMessageAns : BaseMsg {
    private: inline static int m_type = 4;\
    public: int getType() const override {return m_type;}\
    static bool checkType(int type) {return type == m_type;}\
    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
    bool removed;
    void serialize(QDataStream *s) const override;
    void deserialize(QDataStream *s) override;
};

#endif // REMOVEITEMMESSAGE_H
