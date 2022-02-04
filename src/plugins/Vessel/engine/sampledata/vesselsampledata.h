#ifndef SHIP_H
#define SHIP_H

#include <QDataStream>
#include <libs/core/physicalitemsampledata.h>
#include <vessel_global.h>

const int OBJ_SHIP = 2;

struct VESSEL_EXPORT VesselSampleData : public PhysicalItemSampleData
{
public:
    VesselSampleData();
    VesselSampleData(std::vector<double>initPosition);
    VesselSampleData(double velocity, std::vector<double>acceleration);
    VesselSampleData(std::vector<double>initPosition, double velocity, std::vector<double>acceleration, int uSecond);
    VesselSampleData(const VesselSampleData &data);
    //!
    //! \brief Get type
    //! \return QString representation of SampleData
    //!
    QString getType() override;

    std::string to_string() override;

    //!
    //! \brief Deserializes controller sample data from raw binary data
    //!
    void deserialize(QDataStream *stream) override;

    //!
    //! \brief Serializes vesselcontroller sample data into binary data
    //!
    void serialize(QDataStream *stream) const override;
private:
    inline static QString m_type = "Vessel";

    // SampleData interface
public:
    void getAsVariant(QVariantMap &map) override;
};

#endif // SHIP_H
