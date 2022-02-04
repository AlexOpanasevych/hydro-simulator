#ifndef STARTENGINEMSG_H
#define STARTENGINEMSG_H

#include <libs/core/basemsg.h>
#include <pluginsbase_global.h>

enum EngineMode {
    START, PAUSE, RESET
};

struct PLUGINSBASE_EXPORT SetModeEngineMsg : public BaseMsg
{
    private: inline static int m_type = 11;\
    public: int getType() const override {return m_type;}\
    static bool checkType(int type) {return type == m_type;}\
    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
    EngineMode mode;
public:
    void serialize(QDataStream *s) const override;
    void deserialize(QDataStream *s) override;
};

struct PLUGINSBASE_EXPORT SetModeEngineMsgAns : public BaseMsg
{
    private: inline static int m_type = 12;\
    public: int getType() const override {return m_type;}\
    static bool checkType(int type) {return type == m_type;}\
    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
    bool modeChangedSuccess;
    EngineMode mode;
public:
    void serialize(QDataStream *s) const override;
    void deserialize(QDataStream *s) override;
};

#endif // STARTENGINEMSG_H
