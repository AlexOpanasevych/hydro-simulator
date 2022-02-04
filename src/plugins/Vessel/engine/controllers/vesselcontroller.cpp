#include "vesselcontroller.h"

#include <engine/sampledata/vesselcontrollersampledata.h>
//#include <engine/sampledata/torpedosampledata.h>
#include <algorithm>
#include <engine/messages/changecoordsmsg.h>
#include <engine/messages/settorpedoparams.h>
#include <engine/messages/itemstartmsg.h>
#include <math/kmath.h>

VesselController::VesselController()
{
    this->mDisp->registerMsgHandler(&VesselController::proceedSetShipPathMsg, this);
    currentPos = 0;
    timeTaken = 0;
    velocity = 10;
    toAngle = 0;
    goalVelocity = 0;
    fromAngle = 0;
    toAngle = 0;
    zGoal = 0;
    distanceGoal = 0;
    currentDistance = 0;
    acceleration = 0;
    fired = false;

}

bool VesselController::move(PhysicalItemSampleData* shData, double dt)
{
    VesselSampleData * shipData = static_cast<VesselSampleData*>(shData);
    VesselSampleData * shipPrevData = this->e->getPiSamples()->getLastSample<VesselSampleData>(this->piId);
    VesselControllerSampleData * vcPrevData = this->e->getPiSamples()->getLastSample<VesselControllerSampleData>(this->piId);

    if(shipPrevData->destroyed) return false;
    if(getDestroyed()) {
        *shipData = *shipPrevData;
        shipData->destroyed = true;
        return true;
    }
    auto cmdName = currentCmd.name();
    if(cmdName == "FORWARD") {
        if (s == STOPPED) {
            s = RUNNING;
            ItemStartMsg msg;
            msg.id = this->piId;
            msg.itemName = "Vessel";
            this->e->getInterface()->sendFromEngine(msg);
        }
        std::vector<double> velocity;

        velocity = {dir[0] * this->velocity, dir[1] * this->velocity, dir[2] * this->velocity};
        shipData->velocity = this->velocity;

        shipData->position[0] = shipPrevData->position[0] + (velocity[0] * dt);
        shipData->position[1] = shipPrevData->position[1] + (velocity[1] * dt);
        shipData->direction = this->dir;

//        if(currentCmd.getParameter("Z")->modifier == "+") {
//            shipData->position[2] = shipPrevData->position[2] < zGoal ? shipPrevData->position[2] + (velocity[2] * dt) : shipPrevData->position[2];
//        }
//        else if(currentCmd.getParameter("Z")->modifier == "-") {
//            shipData->position[2] = shipPrevData->position[2] > zGoal ? shipPrevData->position[2] - (velocity[2] * dt) : shipPrevData->position[2];
//        }


        currentDistance += this->velocity * dt;

        VesselControllerSampleData * shipCData = new VesselControllerSampleData;
        shipCData->angle = KMath::radiansToDegrees(fromAngle);
        shipCData->stage = vcPrevData->stage;

        this->e->getPiSamples()->pushSample(this->piId, shipCData);

        if(currentDistance > distanceGoal) {
            if(!cmds->isEmpty()) {
                initNextCommand();
                currentDistance = 0;
                return true;
            }
            else {
                if(s == RUNNING) {
                    s = STOPPED;
                    ItemStopMsg msg;
                    msg.id = this->piId;
                    msg.itemName = "Vessel";
                    this->e->getInterface()->sendFromEngine(msg);
                }
                return false;
            }

        }

        return true;

    }
    else if(cmdName == "ROTATE") {

        double dA = this->velocity * dt / currentCmd.getParameter("R")->value;

        //qDebug() << "dA" << dA << "dt" << dt << "angle" << fromAngle;
        auto mod = currentCmd.getParameter("A")->modifier;
        if(mod == "+") {
            fromAngle += dA;
        }
        else if(mod == "-") {
            fromAngle -= dA;
        }
        //        ChangeCoordsMsg msg;
        //        msg.x = shipData->position[0];
        //        msg.y = shipData->position[1];
        //        msg.z = shipData->position[2];

        //        this->e->getInterface()->sendFromEngine(msg, this->piId);

        //        VesselControllerSampleData * vcData = new VesselControllerSampleData;
        //        vcData->angle = angle;
        //        vcData->uSecond = timeTaken;
        //        this->e->getPiSamples()->pushSample(this->piId, vcData);
        setupDirection(KMath::signPiPeriod(fromAngle));
        shipData->direction = this->dir;


        std::vector<double> velocity = {this->velocity * dir[0], this->velocity * dir[1], 0};

        shipData->position[0] = shipPrevData->position[0] + velocity[0] * dt;
        shipData->position[1] = shipPrevData->position[1] + velocity[1] * dt;
        shipData->position[2] = shipPrevData->position[2];

        shipData->velocity = this->velocity;


        VesselControllerSampleData * shipCData = new VesselControllerSampleData;
        shipCData->angle = fromAngle;
        shipCData->stage = vcPrevData->stage;
        this->e->getPiSamples()->pushSample(this->piId, shipCData);

        if(currentCmd.getParameter("A")->modifier == "+") {
            if(fromAngle >= toAngle) {
                if(!cmds->isEmpty()) {
                    initNextCommand();
                    return true;
                }
                else {
                    if(s == RUNNING) {
                        s = STOPPED;
                        ItemStopMsg msg;
                        msg.id = this->piId;
                        msg.itemName = "Vessel";
                        this->e->getInterface()->sendFromEngine(msg);
                    }
                    return false;
                }
            }

        }
        else if(currentCmd.getParameter("A")->modifier == "-") {
            if(fromAngle <= toAngle) {
                if(!cmds->isEmpty()) {
                    initNextCommand();
                    return true;
                }
                else {
                    if(s == RUNNING) {
                        s = STOPPED;
                        ItemStopMsg msg;
                        msg.id = this->piId;
                        msg.itemName = "Vessel";
                        this->e->getInterface()->sendFromEngine(msg);
                    }
                    return false;
                }

            }
        }

        return true;
    }
    else if(cmdName == "FIRE") { // FIRE
        if(!fired) {
            CreatePIMsg msg = this->shoot(shipPrevData);
            Cmd cCmd = currentCmd;
            this->e->getInterface()->sendToEngine(msg).onCompleted<CreatePIMsgAns>([this, cCmd, msg](CreatePIMsgAns answ){
                this->e->getInterface()->sendFromEngine(msg, answ.createdId);
                SetTorpedoParams smsg;
                qDebug() << cCmd.name();
                smsg.firstRA = cCmd.getParameter("FRA")->value;
                if(cCmd.getParameter("SRA")->modifier == "") {

                    smsg.secondRA = cCmd.getParameter("SRA")->value;
                }
                else {
                    smsg.secondRA = -cCmd.getParameter("SRA")->value;
                }
                smsg.torpedoDivergency = cCmd.getParameter("DT")->value;
                this->e->getInterface()->sendToEngine(smsg, answ.createdId).onCompleted<SetTorpedoParamsAns>([](SetTorpedoParamsAns ans){
                    qDebug() << ans.success;
                });
            });

            shipData->position[0] = shipPrevData->position[0];
            shipData->position[1] = shipPrevData->position[1];
            shipData->position[2] = shipPrevData->position[2];

            shipData->velocity = this->velocity;
            shipData->direction = this->dir;

            VesselControllerSampleData * shipCData = new VesselControllerSampleData;
            shipCData->angle = KMath::radiansToDegrees(fromAngle);
            shipCData->stage = vcPrevData->stage;
            this->e->getPiSamples()->pushSample(this->piId, shipCData);

            //this->e->getInterface()->sendToEngine(msg)
            fired = true;
        }
        initNextCommand();
        return true;
    }

    //            using namespace std::complex_literals;
    //            std::complex<double> tmp = 1i * degreesToRadians(1) * (90 - angle);
    //            std::complex<double> z = currentCmd.getParameter("R").value * (angle > 0 ? -1i : 1i);
    //            z *= std::exp(tmp);
    //            center[0] = shipPrevData.position[0] + z.real();
    //            center[1] = shipPrevData.position[1] + z.imag();
    //            fromAngle = 180 + radiansToDegrees(1) * std::arg(z);
    //            toAngle = fromAngle - angle;


    //    bool OnPlace = checkZero(velocity);
    //    if(!path.empty() && !OnPlace){
    //        if(timeTaken <= elapsedTime.back()){
    //            VesselSampleData * shipData = static_cast<VesselSampleData*>(shData);
    //            VesselSampleData shipPrevData = this->e->getPiSamples()->getLastSample<VesselSampleData>(this->piId, 0);
    //            shipData->position[0] = sX(timeTaken);
    //            shipData->position[1] = sY(timeTaken);
    //            shipData->position[2] = sZ(timeTaken);

    //            std::vector<double> line = shipData->position - shipPrevData.position;
    //            double dist = calculateModule(line);
    //            line /= dist;
    //            qDebug() << line;
    //            std::vector<double> j = {0, 1, 0};
    //            double cos = (j * line) / (calculateModule(j) * calculateModule(line));
    //            qDebug() << cos;

    //            if(line[0] > 0)
    //                angle = acos(cos);
    //            else
    //                angle = 2 * M_PI-acos(cos);


    //            VesselControllerSampleData * shipCData = new VesselControllerSampleData;
    //            shipCData->angle = radiansToDegrees(angle);
    //            shipCData->index = 1;
    //            //qDebug() << shipCData->angle << line;
    //            this->e->getPiSamples()->pushSample(this->piId, shipCData);

    //            shipData->velocity[0] = velocityCoords[currentPos][0];
    //            shipData->velocity[1] = velocityCoords[currentPos][1];
    //            shipData->velocity[2] = velocityCoords[currentPos][2];

    //        //    if(path[currentPos][0] == torpedoEjectPoints[currentPos][0] && path[currentPos][1] == torpedoEjectPoints[currentPos][1] && path[currentPos][2] == torpedoEjectPoints[currentPos][3]) {
    //        //        auto [msg, itemId] = this->shoot();
    //        //        //h.uid = this->e->getInterface()->nextMsgIdEngineSide;
    //        //        this->e->getInterface()->sendFromEngine<CreatePIMsg>(msg, itemId);
    //        //    }



    //                        //    if(shipPrevData->position[0] == path[i+1][0] && shipPrevData->position[1] == path[i+1][0])
    //        //        i++;
    //            timeTaken += (double)dt / MAXDT;
    //            return true;
    //        }
    //    }

    shipData->position = shipPrevData->position;
    shipData->velocity = shipPrevData->velocity;
    shipData->direction = shipPrevData->direction;
    shipData->acceleration = shipPrevData->acceleration;

    timeTaken += dt;

    return false;
}


void VesselController::initNextCommand()
{

    if (!cmds->isEmpty())
    {
        currentCmd = *cmds->takeFirst();
        auto cmdName = currentCmd.name();
        VesselControllerSampleData * vPrevData = this->e->getPiSamples()->getLastSample<VesselControllerSampleData>(this->piId);
        VesselControllerSampleData * newShipData = new VesselControllerSampleData;
        newShipData->angle = vPrevData->angle;
        if(cmdName == "FORWARD") {
            QString cmdName = currentCmd.name();


            if(currentCmd.getParameter("V")->modifier == "+")
                goalVelocity += knotToMeters(currentCmd.getParameter("V")->value);
            if(currentCmd.getParameter("V")->modifier == "-")
                goalVelocity -= knotToMeters(currentCmd.getParameter("V")->value);
            if(currentCmd.getParameter("V")->modifier == "") {
                goalVelocity = knotToMeters(currentCmd.getParameter("V")->value);
            }
            if(goalVelocity < 0) goalVelocity = 0;

            distanceGoal = currentCmd.getParameter("D")->value;
            acceleration = currentCmd.getParameter("ACC")->value;

            if(currentCmd.getParameter("Z")->modifier == "+")
                zGoal += currentCmd.getParameter("Z")->value;
            else zGoal -= currentCmd.getParameter("Z")->value;
            newShipData->stage = "Корабель рухається вперед на відстань " + QString::number(distanceGoal) + " м та зі швидкістю " + QString::number(KMath::toKnots(this->velocity)) + " вузлів і під кутом " + QString::number(newShipData->angle) + " градусів";

        }
        if(cmdName == "ROTATE") {
            if(currentCmd.hasParameter("A")) {
//                initStartAngle();
                double angleRad = KMath::degreesToRadians(currentCmd.getParameter("A")->value);
                if(currentCmd.getParameter("A")->modifier == "+") {
                    toAngle = fromAngle + angleRad;
                    newShipData->stage = "Корабель повертає вправо на курсовий кут " + QString::number(KMath::radiansToDegrees(toAngle)) + " градусів";
                }
                else {
                    toAngle = fromAngle - angleRad;
                    newShipData->stage = "Корабель повертає вліво на курсовий кут " + QString::number(KMath::radiansToDegrees(toAngle)) + " градусів";

                }
            }
        }
        if(cmdName == "FIRE") {
            fired = false;
            if(currentCmd.hasParameter("P")) {

                pelengAngle = currentCmd.getParameter("P")->value;
                newShipData->stage = "Випускається торпеда під кутом " + QString::number(pelengAngle) + " градусів";
            }
        }

        this->e->getPiSamples()->pushSample<>(this->piId, newShipData);
    }
    else {
        if(s == RUNNING) {
            s = STOPPED;
            ItemStopMsg msg;
            msg.id = this->piId;
            msg.itemName = "Vessel";
            this->e->getInterface()->sendFromEngine(msg);
        }
    }
}

void VesselController::initFirstCommand()
{
    if(currentCmd.hasParameter("V")){
        if(currentCmd.getParameter("V")->modifier == "+")
            goalVelocity += knotToMeters(currentCmd.getParameter("V")->value);;
        if(currentCmd.getParameter("V")->modifier == "-")
            goalVelocity -= knotToMeters(currentCmd.getParameter("V")->value);;
        if(currentCmd.getParameter("V")->modifier == "") {
            goalVelocity = knotToMeters(currentCmd.getParameter("V")->value);;
        }
        if(goalVelocity < 0) goalVelocity = 0;

    }
    if(currentCmd.hasParameter("D"))
        distanceGoal = currentCmd.getParameter("D")->value;
    if(currentCmd.hasParameter("ACC"))
        acceleration = currentCmd.getParameter("ACC")->value;
    if(currentCmd.hasParameter("Z")) {

        if(currentCmd.getParameter("Z")->modifier == "+")
            zGoal += currentCmd.getParameter("Z")->value;
        else zGoal -= currentCmd.getParameter("Z")->value;
    }

    if(currentCmd.hasParameter("A")) {
//        initStartAngle();
        double angleRad = KMath::degreesToRadians(currentCmd.getParameter("A")->value);
        toAngle = currentCmd.getParameter("A")->modifier == "+" ? fromAngle + angleRad : fromAngle - angleRad;
    }
}

void VesselController::proceedAfter(long long uSecond, double)
{

}

QVariantMap VesselController::getInfo()
{
    auto vesselData = this->e->getPiSamples()->getLastSample<VesselSampleData>(piId);
    auto vesselCData = this->e->getPiSamples()->getLastSample<VesselControllerSampleData>(piId);
    return QVariantMap{
        {"objectType", vesselData->getType()},
        {"x", vesselData->position[0]},
        {"y", vesselData->position[1]},
        {"velocity", vesselData->velocity},
        {"xDirection", vesselData->direction[0]},
        {"yDirection", vesselData->direction[1]},
        {"rotation", vesselCData->angle}
    };
}

QString VesselController::name()
{
    return "Vessel";
}

void VesselController::proceed(long long uSecond, double dt)
{
   VesselSampleData * shipData = new VesselSampleData;
        shipData->uSecond = uSecond;

        double timeInSeconds = dt / MCS_IN_S;
    //    if(this->e->doMath) {
        if(this->move(shipData, timeInSeconds))
            this->e->getPiSamples()->pushSample<VesselSampleData>(this->piId, shipData);
        else delete shipData;

    //    qDebug() << "using getsample" << this->e->getPiSamples()->getSample<VesselSampleData>(this->piId, uSecond)->uSecond;
    //    qDebug() << "using getlastsample" << this->e->getPiSamples()->getLastSample<VesselSampleData>(this->piId)->uSecond;
    //    }
    //    else delete shipData;
        timeTaken += timeInSeconds;

}

/*
FORWARD D1500 V+30.3 ACC10;
ROTATE A+35 R600;
ROTATE A-65 R1000;
ROTATE A+60 R800;
ROTATE A-40 R600;
ROTATE A+30 R6000;
FORWARD D1000 V-30 D10;


ROTATE A+10 R6000;
ROTATE A+35 R600;
ROTATE A-65 R1000;
ROTATE A+60 R800;
ROTATE A-40 R600;
ROTATE A+30 R6000;
FORWARD D1000 V-30 D10;
*/

CreatePIMsg VesselController::shoot(VesselSampleData* data)
{
    //TorpedoSampleData * newTorpedo = new TorpedoSampleData(data->position, 30, {0, 0, 0}, 0);
    //this->e->getPiSamples()->pushSample<TorpedoSampleData>(this->e->lastCreatedPIID, newTorpedo);
    //VesselControllerSampleData * vcData = this->e->getPiSamples()->getLastSample<VesselControllerSampleData>(this->piId);
    CreatePIMsg msg;
    msg.position[0] = data->position[0];
    msg.position[1] = data->position[1];
    msg.position[2] = data->position[2];
    msg.velocity = KMath::fromKnots(40); // knots
    msg.typeOfObject = "Torpedo";
    msg.angle = this->pelengAngle;
    return msg;
}

SetShipPathMsgAns VesselController::proceedSetShipPathMsg(SetShipPathMsg msg)
{
    TrajectoryParser parser;
    this->cmds = parser.parse(msg.cmds);
    //    this->path = msg.path;
    //    this->velocity = msg.velocity;
    //    this->angleTorpedoRelease = msg.angle;
    //    this->torpedoEjectPoints = msg.torpedoEjectPoints;
    //    this->calcSpline();
//    VesselControllerSampleData * vPrevData = this->e->getPiSamples()->getLastSample<VesselControllerSampleData>(this->piId);
//    setupDirection(KMath::degreesToRadians(vPrevData->angle));
//    fromAngle = KMath::degreesToRadians(vPrevData->angle);
    initNextCommand();

    SetShipPathMsgAns msgAns;
    if(!cmds->isEmpty())
        msgAns.success = true;
    else msgAns.success = false;
    return msgAns;
}

//void VesselController::calcSpline()
//{
//    elapsedTime.push_back(0);
//    for (unsigned int i = 0; i < path.size() - 1; ++i) {

//        std::vector<double> tau;
//        std::vector<double> tauNext;

//        tau = {(path[i+1][0] - path[i][0]), (path[i+1][1] - path[i][1]), (path[i+1][2] - path[i][2])};

//        if(i < path.size() - 2) {
//            tauNext = {(path[i+2][0] - path[i+1][0]), (path[i+2][1] - path[i+1][1]), (path[i+2][2] - path[i+1][2])};
//        }
//        else {
//            //std::array<double, 3> n = {path[i + 1][0] - path[i][0], path[i + 1][1] - path[i][1], path[i + 1][2] - path[i][2]};
//            std::array<double, 3> nextPoint = {path[i][0] + tau[0] * elapsedTime.back(), path[i][1] + tau[1] * elapsedTime.back(), path[i][2] + tau[2] * elapsedTime.back()};
//            tauNext = {nextPoint[0] - path[i + 1][0], nextPoint[0] - path[i + 1][0], nextPoint[0] - path[i + 1][0]};
//        }

//        double D = calculateModule(tau);
//        tau[0] /= D;
//        tau[1] /= D;
//        tau[2] /= D;

//        double dNext = calculateModule(tauNext);
//        tauNext[0] /= dNext;
//        tauNext[1] /= dNext;
//        tauNext[2] /= dNext;

//        if(i == path.size() - 2) {
//            velocityCoords.push_back({velocity[i] * tau[0], velocity[i] * tau[0], velocity[i] * tau[0]});
//        }
//        else {
//            velocityCoords.push_back({velocity[i] * tau[0], velocity[i] * tau[1], velocity[i] * tau[2]});
//        }

//        //std::vector<double> currPointVelocity = {velocity[i] * tau[0], velocity[i] * tau[1], velocity[i] * tau[2]};
//        //std::vector<double> nextPointVelocity = {velocity[i+1] * tauNext[0], velocity[i+1] * tauNext[1], velocity[i+1] * tauNext[2]};

//        int sum = 0;
//        for(int i = 1; i < COUNT_OF_POINTS; i++){
//            sum += i;
//        }

//        double deltaT = abs(COUNT_OF_POINTS * (calculateModule({path[i+1][0], path[i+1][1], path[i+1][2]}) - calculateModule({path[i][0], path[i][1], path[i][2]})) / (COUNT_OF_POINTS * velocity[i] + ((velocity[i+1] - velocity[i])/COUNT_OF_POINTS) * sum));

//        elapsedTime.push_back(elapsedTime.back() + deltaT);
//    }
//    std::vector<double> xAxis;
//    std::vector<double> yAxis;
//    std::vector<double> zAxis;
//    for (unsigned int i = 0; i < path.size(); ++i) {
//        xAxis.push_back(path[i][0]);
//        yAxis.push_back(path[i][1]);
//        zAxis.push_back(path[i][2]);
//    }
//    sX.set_points(elapsedTime, xAxis);
//    sY.set_points(elapsedTime, yAxis);
//    sZ.set_points(elapsedTime, zAxis);
//}





void VesselController::setupDirection(double angle)
{
//    if(angle < 0) {
//        angle += 2 * M_PI;
//    }
    this->dir[0] = sin(angle);
    this->dir[1] = cos(angle);
    this->dir[2] = 0;
}

void VesselController::initStartAngle()
{
    fromAngle = KMath::signPiPeriod(KMath::getAngle(dir[0], dir[1]));
}

void VesselController::init(BaseMsg *msg, AbstractEngine *e)
{
    setEngine(e);
    CreatePIMsg * itemData = static_cast<CreatePIMsg*>(msg);
    VesselSampleData * vData = new VesselSampleData;
    this->e->getPiSamples()->CreateNewRecord(this->piId, vData->getType(), false);
    vData->position[0] = itemData->position[0];
    vData->position[1] = itemData->position[1];
    vData->position[2] = itemData->position[2];
    vData->velocity = itemData->velocity;
    fromAngle = KMath::degreesToRadians(itemData->angle);
    setupDirection(KMath::signPiPeriod(KMath::degreesToRadians(itemData->angle)));
    vData->direction = dir;
    vData->uSecond = 0;
    vData->acceleration = {0, 0, 0};
    this->e->getPiSamples()->pushSample(this->piId, vData);
    VesselControllerSampleData * vcData = new VesselControllerSampleData;
    vcData->angle = itemData->angle;
    vcData->uSecond = 0;
    this->e->getPiSamples()->CreateNewRecord(this->piId, vcData->getType(), false);
    this->e->getPiSamples()->pushSample(this->piId, vcData);
}

std::vector<double> operator -(const std::vector<double> &left, const std::vector<double> &right)
{
    return {right[0] - left[0], right[1] - left[1], right[2] - left[2]};
}

double operator *(const std::vector<double> &left, const std::vector<double> &right)
{
    return left[0] * right[0] + left[1] * right[1] + left[2] * right[2];
}

void operator /=(std::vector<double> &vec, double number)
{
    vec[0] /= number;
    vec[1] /= number;
    vec[2] /= number;
}

bool checkZero(const std::vector<double> &vec)
{
    return std::all_of(vec.begin(), vec.end(), [vec](double x){ return x==0.0; });
}
