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

#define MAXLIFE_SEC_TORPEDO 40.

class TORPEDO_EXPORT TorpedoController : public BaseMoveController, public Scanable
{
    double onLife;
    double t_rot;
    double teta0;
    double w;
    double omega;
    double alpha;

    const double distanceToTrajectory = 250.0;
    const double wRadius = 70.0;

    double torpedoDivergency;
    double firstAndSecondDistance;
    double firstRA;
    double secondRA;
    double toAngle;
    double fromAngle;
    double currentDistance;

    bool secondStage;
    bool thirdStage;
    bool fourthStage;
    QString stage;

    double maxLeft;
    double maxRight;
    bool circulation;
    bool setParams;

    void initStartAngle();

    //double depth;
    //std::vector<double> time;
    //void rotate(int dt);
    ChangeAngleMsgAns proceedChangeAngle(ChangeAngleMsg msg);
    ChangeCoordsMsgAns proceedChangeCoords(ChangeCoordsMsg msg);
    SetTorpedoParamsAns proceedSetPath(SetTorpedoParams msg);
    QList<Cmd*> * cmds;
    std::array<double, 3> dir;
public:
    TorpedoController();
    void scan(const std::map<int, double> & objects, int itemId) override;
    bool move(PhysicalItemSampleData *sData, double) override;
    void proceed(double) override;
    BaseMsg * proceedMsg(BaseMsg * msg);
    void resetAzimuth();
    void init(BaseMsg * msg, AbstractEngine * e) override;
    void setup(double firstRA = 0, double sRA = 0, double torpedoDivergency = 15000);
    void setupDirection(double angle);
    //void initStartAngle();
    std::vector<double> count_next_position(double x, double y, double z, double t0, double dt, double vy);
    //void set_main_coord(PhysicalItemSampleData * tData);
};

#endif // TORPEDOCONTROLLER_H
