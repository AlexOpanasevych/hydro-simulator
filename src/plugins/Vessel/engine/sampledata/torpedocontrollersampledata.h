#ifndef TORPEDOCONTROLLERSAMPLEDATA_H
#define TORPEDOCONTROLLERSAMPLEDATA_H

#include <libs/core/sampledata.h>
#include <torpedo_global.h>

struct TORPEDO_EXPORT TorpedoControllerSampleData : SampleData
{
//private: inline static int m_type = 4;
//public: int getType() const override {return m_type;}
//static bool checkType(int type) {return type == m_type;}
//static bool checkType(SampleData *msg) {return msg->getType() == m_type;}
    int piId;
    double alpha;
    double theta;

    QString stage;
    std::string to_string() override;
    QString getType() override;
private:
    inline static QString m_type = "TorpedoController";
public:
    TorpedoControllerSampleData();
};

#endif // TORPEDOCONTROLLERSAMPLEDATA_H
