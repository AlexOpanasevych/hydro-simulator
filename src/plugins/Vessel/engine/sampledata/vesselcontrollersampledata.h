#ifndef VESSELCONTROLLERSAMPLEDATA_H
#define VESSELCONTROLLERSAMPLEDATA_H

#include <vector>
#include <libs/core/sampledata.h>
#include <vessel_global.h>

struct VESSEL_EXPORT VesselControllerSampleData : public SampleData
{
    double angle;
    QString stage;
    //int theta;
    static inline QString m_type = "VesselController";
public:
    VesselControllerSampleData();

    //!
    //! \brief Get type
    //! \return QString representation of SampleData
    //!
    QString getType() override;
    std::string to_string() override;

    //!
    //! \brief Deserializes controller sample data from raw binary data
    //!
    void deserialize(QDataStream * s) override;

    //!
    //! \brief Serializes vesselcontroller sample data into binary data
    //!
    void serialize(QDataStream * s) const override;


    // SampleData interface
public:
    void getAsVariant(QVariantMap &map) override;
};

#endif // VESSELCONTROLLERSAMPLEDATA_H
