#ifndef TORPEDOSAMPLEDATA_H
#define TORPEDOSAMPLEDATA_H

#include <QDataStream>
#include <libs/core/physicalitemsampledata.h>
#include <torpedo_global.h>


struct TORPEDO_EXPORT TorpedoSampleData : public PhysicalItemSampleData
{
    TorpedoSampleData(std::vector<double>initPosition);
    TorpedoSampleData(double velocity, std::vector<double>acceleration);
    TorpedoSampleData(std::vector<double>initPosition, double velocity, std::vector<double>acceleration, int uSecond);
    TorpedoSampleData(const TorpedoSampleData &data);


    double angleVelocity = 0;
    int wIndex = 0;
    double distanceToGoal = 0;

    double currentDistance = 0;

    double beta = 0;
    double onLife = 0;
    bool goalFound = false;

    //!
    //! \brief Converts \c this object to it's string representation
    //! \return string representation
    //!
    std::string to_string() override;

    //!
    //! \brief Get type
    //! \return string type representation
    //!
    QString getType() override;
    TorpedoSampleData();

    //!
    //! \brief Deserializes message from \p stream
    //! \param stream
    //!
    void deserialize(QDataStream * stream) override;

    //!
    //! \brief Serializes message into \p stream
    //! \param stream
    //!
    void serialize(QDataStream * stream) const override;
private:
    inline static QString m_type = "Torpedo";

    // SampleData interface
public:
    void getAsVariant(QVariantMap &map) override;
};





#endif // TORPEDOSAMPLEDATA_H
