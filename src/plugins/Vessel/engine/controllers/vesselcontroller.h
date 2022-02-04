#ifndef VESSELCONTROLLER_H
#define VESSELCONTROLLER_H

#include <libs/core/spline.h>
#include <libs/core/basemovecontroller.h>
#include <libs/core/basemovecontroller.h>
#include <engine/messages/setshippathmsg.h>
#include <libs/PluginsBase/tools/cmd.h>
#include <libs/PluginsBase/tools/trajectoryparser.h>
#include <libs/PluginsBase/base/global_messages/createpimsg.h>
#include <engine/sampledata/vesselsampledata.h>
#include <vessel_global.h>

#define COUNT_OF_POINTS 50

class VESSEL_EXPORT VesselController : public BaseMoveController {
//    std::vector<std::array<double, 3>> path;
//    std::vector<std::array<double, 3>> torpedoEjectPoints;
    double velocity;

    enum VesselState {RUNNING, STOPPED};
    VesselState s = VesselState::STOPPED;

    std::vector<double> elapsedTime;
    int currentPos;
    double timeTaken;
    Cmd currentCmd;
    QList<Cmd *> * cmds;
    double distanceGoal;
    double zGoal;
    double currentDistance;
    double toAngle;
    double fromAngle;
    double goalVelocity;
    double acceleration;
    std::array<double, 3> dir;
    double pelengAngle;
    bool fired;
public:
    VesselController();

    //!
    //! \brief Proceeds move routine
    //! \return \c true if succeed, \c false otherwise
    //!
    bool move(PhysicalItemSampleData*, double) override;

    //!
    //! \brief Proceeds current tick in engine
    //! \param dt
    //!
    void proceed(long long uSecond , double dt) override;

    //!
    //! \brief Routine that produces new torpedo
    //! \param data
    //! \return CreatePIMsg object
    //!
    CreatePIMsg shoot(VesselSampleData *data);

    //!
    //! \brief Proceeds SetShipPathMsg \c msg, gets commands for vessel
    //! \param msg
    //! \return answer to \c msg message
    //!
    SetShipPathMsgAns proceedSetShipPathMsg(SetShipPathMsg msg);
//    void calcSpline();

    //!
    //! \brief Sets new direction with given \p angle
    //! \param angle
    //!
    void setupDirection(double angle);

    //!
    //! \brief Initializes start angle for vessel
    //!
    void initStartAngle();

    //!
    //! \brief Initializes controller with given \p msg, and \p e engine
    //! \param msg
    //! \param e
    //!
    void init(BaseMsg * msg, AbstractEngine *e) override;
private:
    //!
    //! \brief Initializes next command
    //!
    void initNextCommand();

    //!
    //! \brief Initializes first command with special logic
    //!
    void initFirstCommand();

    // AbstractPIController interface
public:
    void proceedAfter(long long uSecond, double) override;

    QVariantMap getInfo() override;


    // AbstractPIController interface
public:
    QString name() override;
};

//!
//! \brief Substracts vector \p left by another \p right
//! \param left
//! \param right
//! \return
//!
std::vector<double> operator -(const std::vector<double> & left, const std::vector<double> & right);

//!
//! \brief Multiplies vector \p left by another \p right vector
//! \param left
//! \param right
//! \return
//!
double operator *(const std::vector<double> & left, const std::vector<double> & right);

//!
//! \brief Divides vector \p vec by given \p number
//! \param vec
//! \param number
//!
void operator /=(std::vector<double> & vec, double number);

//!
//! \brief Checks if vector \p vec is zero-vector
//! \param vec
//! \return
//!
bool checkZero(const std::vector<double> & vec);

#endif // VESSELCONTROLLER_H
