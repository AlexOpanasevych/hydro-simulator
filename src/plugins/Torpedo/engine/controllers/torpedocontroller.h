#ifndef TORPEDOCONTROLLER_H
#define TORPEDOCONTROLLER_H

#include <libs/core/messagedispatcher.h>
#include <libs/core/scanable.h>
#include <engine/messages/changeanglemsg.h>
#include <engine/messages/changecoordsmsg.h>
#include <engine/messages/setshippathmsg.h>
#include <engine/messages/settorpedoparams.h>
#include <engine/sampledata/torpedosampledata.h>
#include <libs/core/basemovecontroller.h>
#include <torpedo_global.h>
#include <libs/PluginsBase/tools/cmd.h>

#define MAXLIFE_SEC_TORPEDO 400.


class TORPEDO_EXPORT TorpedoController : public BaseMoveController, public Scanable
{
    enum TorpedoState {LeftCirculating, RightCirculating};
    double onLife;
    double t_rot;
    double teta0;
    std::array<double, 4> w;
    double omega;
    double alpha;
    const double sector;
    double afterLostDistance = 0;
    const double afterLostMaxDistance = 500;
    TorpedoState circulating;

    double distanceToTrajectory = 250.0;
    const double wRadius = 70.0;

    const double minDistanceToAttack = 1000.;
    const double killDistance = 13.;

    double distanceToGoal = 0;
    int goalID = -1;

//    double distance = 0;
    double beta = 0;



    double torpedoDivergency;
    double firstAndSecondDistance;
    double firstAndSecondHalfDistance;
    double firstRA;
    double secondRA;
    double toAngle;
    double fromAngle;
    double currentDistance;

    bool secondStage;
    bool thirdStage;
    bool fourthStage;
    bool fifthStage;
    QString stage;

    double maxLeft;
    double maxRight;
    bool circulation;
    bool setParams;


    void initStartAngle();

    //double depth;
    //std::vector<double> time;
    //void rotate(int dt);
    //!
    //! \brief Proceeds change angle message
    //! \param msg
    //! \return answer to message
    //!
    ChangeAngleMsgAns proceedChangeAngle(ChangeAngleMsg msg);

    //!
    //! \brief Proceeds change coords message
    //! \param msg
    //! \return answer to message
    //!
    ChangeCoordsMsgAns proceedChangeCoords(ChangeCoordsMsg msg);

    //!
    //! \brief Proceeds set path message
    //! \param msg
    //! \return answer to message
    //!
    SetTorpedoParamsAns proceedSetPath(SetTorpedoParams msg);
    QList<Cmd*> * cmds;
    std::array<double, 3> dir;
public:
    TorpedoController();

    //!
    //! \brief Implements Scanable interface function \c scan()
    //! \param objects
    //! \param itemId
    //!
    void scan(const std::map<int, double> & objects, int itemId) override;

    //!
    //! \brief Implements BaseMoveController function \c move()
    //! \param sData
    //! \param dt
    //! \return \c true if succeed, \c false otherwise
    //!
    bool move(PhysicalItemSampleData *sData, double dt) override;

    //!
    //! \brief Proceeds current tick in engine
    //!
    void proceed(long long, double) override;

    void resetAzimuth();

    //!
    //! \brief Initiaizes controller wi
    //! \param msg
    //! \param e
    //!
    void init(BaseMsg * msg, AbstractEngine * e) override;

    //!
    //! \brief Initializes controller with start parameters
    //! \param firstRA - first rotation angle
    //! \param sRA - second rotation angle
    //! \param torpedoDivergency
    //!
    void setup(double firstRA, double sRA, double torpedoDivergency, double rotateDistance);

    //!
    //! \brief Setups direction by given \p angle
    //! \param angle
    //!
    void setupDirection(double angle);
    //void initStartAngle();

    //!
    //! \brief Calculates next torpedo position
    //! \param x
    //! \param y
    //! \param z
    //! \param t0
    //! \param dt
    //! \param vy
    //! \return
    //!
    std::vector<double> count_next_position(double x, double y, double z, double t0, double dt, double vy);

    // AbstractPIController interface
public:
    void proceedAfter(long long, double) override;

    // AbstractPIController interface
public:
    QVariantMap getInfo() override;

    // AbstractPIController interface
public:
    QString name() override;
};

#endif // TORPEDOCONTROLLER_H
