#ifndef SETTORPEDOPARAMS_H
#define SETTORPEDOPARAMS_H

#include <basemsg.h>
#include <pluginsbase_global.h>

struct PLUGINSBASE_EXPORT SetTorpedoParams : public BaseMsg
{
public:
    SetTorpedoParams();
    double firstRA;
    double secondRA;
    double torpedoDivergency;
    double rotateDistance;
private: inline static int m_type = 15;\
public:
    void serialize(QDataStream *s) const override;
    void deserialize(QDataStream *s) override;

    int getType() const override {return m_type;}\
    static bool checkType(int type) {return type == m_type;}\
    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
};

struct PLUGINSBASE_EXPORT SetTorpedoParamsAns : public BaseMsg
{
public:
    SetTorpedoParamsAns();
    bool success;
private: inline static int m_type = 16;\

public:
    void serialize(QDataStream *s) const override;
    void deserialize(QDataStream *s) override;

    int getType() const override {return m_type;}\
    static bool checkType(int type) {return type == m_type;}\
    static bool checkType(BaseMsg *msg) {return msg->getType() == m_type;}
};

#endif // SETTORPEDOPARAMS_H
