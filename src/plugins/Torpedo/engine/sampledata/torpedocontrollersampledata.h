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

    //!
    //! \brief Converts \c this object to it's string representation
    //! \return string representation
    //!
    std::string to_string() override;

    //!
    //! \brief Get type
    //! \return QString type representation
    //!
    QString getType() override;
private:
    inline static QString m_type = "TorpedoController";
public:
    TorpedoControllerSampleData();

    // SampleData interface
public:
    void getAsVariant(QVariantMap &map) override;
};

#endif // TORPEDOCONTROLLERSAMPLEDATA_H
