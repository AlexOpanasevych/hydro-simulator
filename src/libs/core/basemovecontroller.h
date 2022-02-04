#ifndef BASEMOVECONTROLLER_H
#define BASEMOVECONTROLLER_H

#include <math.h>
#include <libs/core/abstractpicontroller.h>
#include <libs/core/physicalitemsampledata.h>
#include "core_global.h"

#define MCS_IN_S 1000000.
#define MAXSIDE 2 * MCS_IN_S + 3000000
//#define degreesToRadians(angleDegrees) (angleDegrees * M_PI / 180.0)

//#define radiansToDegrees(angleRadians) (angleRadians * 180.0 / M_PI)

#define knotToMeters(meters) (meters * 0.51)


class CORE_LIBRARY_EXPORT BaseMoveController : public AbstractPIController
{
//    double taskAzimuth;
//    double theta;
    bool destroyed = false;
//    verticalCirculation vCirc;
//    horizontalCirculation hCirc;
public:
    BaseMoveController();
    virtual ~BaseMoveController();
    //!
    //! \brief Proceeds move routine, which is overloaded in every physical item controller
    //! \return \a true if succeed, \a false if not
    //!
    virtual bool move(PhysicalItemSampleData *, double) = 0;
    bool getDestroyed() const;
    void setDestroyed(bool newDestroyed);
};

//!
//! \brief Helper function to calculate module for vector \a vec
//! \param vec
//! \return Module for given vector \a vec
//!
double calculateModule(std::vector<double> vec);

//!
//! \brief Normalizes given vector \a vec
//! \param vec
//! \return Normalized vector
//!
std::array<double, 3> normalize(std::array<double, 3> & vec);

//!
//! \brief Calculates goal angle by given \a angle, angular velocity and time \a dt
//! \param angle
//! \param angularVelocity
//! \param dt
//! \return Returns next goal angle
//!
double calcAngle(double angle, double angularVelocity, int dt);
std::vector<double> normalize(std::vector<double> vec);

#endif // BASEMOVECONTROLLER_H
