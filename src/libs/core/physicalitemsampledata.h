#ifndef PHYSICALITEMSAMPLEDATA_H
#define PHYSICALITEMSAMPLEDATA_H

#include "sampledata.h"

#include "core_global.h"
#include <vector>
#include <string>

//!
//! \brief The PhysicalItemSampleData struct
//!
struct CORE_LIBRARY_EXPORT PhysicalItemSampleData : public SampleData
{
    PhysicalItemSampleData();
    PhysicalItemSampleData(std::vector<double>initPosition, double velocity, std::vector<double>acceleration, int uSecond);
    virtual ~PhysicalItemSampleData();

    //!
    //! \brief Get type of sample data
    //! \return QString type of \a this
    //!
    QString getType() override { return ""; }
    std::vector<double> position;
    std::array<double, 3> direction;
    double velocity; // m/s
    std::vector<double> acceleration;
    bool destroyed = false;

    //!
    //! \brief Sets new position value
    //! \param newPosition
    //!
    void setPosition(std::vector<double> newPosition);

    //!
    //! \brief Sets new absolute velocity value
    //! \param newVelocity
    //!
    void setVelocity(double newVelocity);

    //!
    //! \brief Sets new acceleration value
    //! \param newAcceleration
    //!
    void setAcceleration(std::vector<double> newAcceleration);

    //!
    //! \brief Converts \a this to std::string
    //! \return string that represents \a this
    //!
    std::string to_string() override {return "";}

    //!
    //! \brief Deserializes \a this sample data from raw binary data
    //!
    void deserialize(QDataStream *) override;

    //!
    //! \brief Serializes sample data into binary data
    //!
    void serialize(QDataStream *) const override;

    // SampleData interface
public:
    void getAsVariant(QVariantMap &map) override;
};


std::vector<double> CORE_LIBRARY_EXPORT operator *(const std::vector<double> & left, double right);
void CORE_LIBRARY_EXPORT operator +=(std::vector<double> & left, std::vector<double> & right);
void CORE_LIBRARY_EXPORT operator *=(std::vector<double> & left, std::vector<double> & right);
std::vector<double>  CORE_LIBRARY_EXPORT operator +(const std::vector<double> & left, const std::vector<double> & right);
#endif // PHYSICALITEMSAMPLEDATA_H
