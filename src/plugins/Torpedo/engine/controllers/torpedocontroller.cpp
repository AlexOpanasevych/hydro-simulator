#include "torpedocontroller.h"

#include <libs/core/abstractengine.h>
#include <QDateTime>
#include <QDebug>
#include <engine/sampledata/torpedocontrollersampledata.h>
#include <engine/sampledata/torpedosampledata.h>
#include <libs/core/physicalitemsampledata.h>
#include <base/global_messages/createpimsg.h>
#include <tools/trajectoryparser.h>
#include <math/kmath.h>
#include <cdebug.h>

SetTorpedoParamsAns TorpedoController::proceedSetPath(SetTorpedoParams msg)
{

    qDebug(s) << "i'm in torpController";
    setup(KMath::degreesToRadians(msg.firstRA), KMath::degreesToRadians(msg.secondRA), msg.torpedoDivergency, msg.rotateDistance);
    SetTorpedoParamsAns answ;
    answ.success = true;
    return answ;
}

ChangeAngleMsgAns TorpedoController::proceedChangeAngle(ChangeAngleMsg msg)
{
    Q_UNUSED(msg)
    ChangeAngleMsgAns newAnswer;
    newAnswer.changed = true;
    // there we can some info I don't know what
    return newAnswer;
}

ChangeCoordsMsgAns TorpedoController::proceedChangeCoords(ChangeCoordsMsg msg)
{
    Q_UNUSED(msg)
    return ChangeCoordsMsgAns();
}

TorpedoController::TorpedoController() : sector(KMath::degreesToRadians(20.))
{
    onLife = 0;
    teta0 = KMath::degreesToRadians(30);
    t_rot = 4.29;
    w[0] = 0;
    w[1] = KMath::degreesToRadians(3);
    w[2] = KMath::degreesToRadians(7);
    w[3] = KMath::degreesToRadians(11);
    omega = 0;
    alpha = 0;
    currentDistance = 0;
    secondStage = false;
    thirdStage = false;
    fourthStage = false;
    stage = "";

    maxLeft = 0;
    maxRight = 0;

    setParams = false;

    this->mDisp->registerMsgHandler(&TorpedoController::proceedChangeCoords, this);
    this->mDisp->registerMsgHandler(&TorpedoController::proceedChangeAngle, this);
    this->mDisp->registerMsgHandler(&TorpedoController::proceedSetPath, this);
}

void TorpedoController::scan(const std::map<int, double> & objects, int itemId)
{
    for(auto & item : objects){
        if(item.first == itemId) continue;
        // TODO : make scan, create vector ray
    }
}

bool TorpedoController::move(PhysicalItemSampleData *sData, double dt)
{

    if(setParams) {
        auto shipData = static_cast<TorpedoSampleData*>(sData);
        TorpedoSampleData * sample = this->e->getPiSamples()->getLastSample<TorpedoSampleData>(this->piId);
//        TorpedoControllerSampleData *sampleC = this->e->getPiSamples()->getLastSample<TorpedoControllerSampleData>(this->piId);

        if(onLife <= MAXLIFE_SEC_TORPEDO) {

            if(sample->destroyed) return false;
            if(this->getDestroyed()) {
                *shipData = *sample;
                shipData->wIndex = 0;
                shipData->angleVelocity = w[0];
                shipData->destroyed = true;
                return true;
            }

            if(currentDistance < distanceToTrajectory and not secondStage and not fifthStage and goalID == -1) {
                double dA = w[2] * dt;
                //std::vector<double> velocity = {sample->velocity * dir[0], sample->velocity * dir[1], 0};
                if(fromAngle > toAngle) {
                    circulating = LeftCirculating;
                    fromAngle -= dA;
                    shipData->angleVelocity = -w[2];
                    shipData->wIndex = -2;

                    setupDirection(this->fromAngle);

                }

                else if(fromAngle < toAngle) {
                    circulating = RightCirculating;
                    fromAngle += dA;
                    shipData->angleVelocity = w[2];
                    shipData->wIndex = 2;

                    setupDirection(this->fromAngle);

                }

                if(currentDistance + sample->velocity * dt > distanceToTrajectory  and goalID == -1) {
                    secondStage = true;
                    toAngle += secondRA;
                    qDebug(s) << "end first stage";
//                    stage = "Другий етап: другий поворот на " + QString::number(KMath::radiansToDegrees(secondRA)) + " градусів і проходження дистанції розходження " + QString::number(torpedoDivergency) + " метрів";
                }

                std::vector<double> velocity = {dir[0] * sample->velocity, dir[1] * sample->velocity, dir[2] * sample->velocity};

                shipData->position[0] = sample->position[0] + velocity[0] * dt;
                shipData->position[1] = sample->position[1] + velocity[1] * dt;
                shipData->position[2] = sample->position[2];

                shipData->velocity = sample->velocity;

                shipData->distanceToGoal = this->distanceToGoal;
                shipData->beta = this->beta;

                currentDistance += sample->velocity * dt;
                shipData->currentDistance = currentDistance;


                TorpedoControllerSampleData * sData = new TorpedoControllerSampleData;

                sData->alpha = this->fromAngle;
                sData->stage = stage;
                //qDebug() << this->fromAngle;

                this->e->getPiSamples()->pushSample(this->piId, sData);

            }


            else if(torpedoDivergency != 0 && currentDistance > distanceToTrajectory and not thirdStage and not fifthStage and goalID == -1) {
                double dA = w[3] * dt;
                //std::vector<double> velocity = {sample->velocity * dir[0], sample->velocity * dir[1], 0};

                if(fromAngle < toAngle) {
                    circulating = RightCirculating;
                    fromAngle += dA;
                    shipData->angleVelocity = w[3];
                    shipData->wIndex = 3;

                    setupDirection(this->fromAngle);
                }

                else if(fromAngle > toAngle) {
                    circulating = LeftCirculating;
                    fromAngle -= dA;
                    shipData->angleVelocity = -w[3];
                    shipData->wIndex = -3;

                    setupDirection(this->fromAngle);

                }
                if(currentDistance + sample->velocity * dt > firstAndSecondHalfDistance) {
                    thirdStage = true;
                    toAngle -= secondRA;
                    qDebug() << "end 2 stage";
//                    stage = "Третій етап: розворот на " + QString::number(secondRA) + " градусів і проходження дистанції розходження " + QString::number(torpedoDivergency);
                }
                std::vector<double> velocity = {dir[0] * sample->velocity, dir[1] * sample->velocity, dir[2] * sample->velocity};

                shipData->position[0] = sample->position[0] + velocity[0] * dt;
                shipData->position[1] = sample->position[1] + velocity[1] * dt;
                shipData->position[2] = sample->position[2];

                shipData->velocity = sample->velocity;

                shipData->distanceToGoal = this->distanceToGoal;

                currentDistance += sample->velocity * dt;

                shipData->currentDistance = currentDistance;
                shipData->beta = this->beta;


                TorpedoControllerSampleData * sData = new TorpedoControllerSampleData;

                sData->alpha = this->fromAngle;
                sData->stage = stage;
                //qDebug() << this->fromAngle;

                this->e->getPiSamples()->pushSample(this->piId, sData);


            }

            else if(torpedoDivergency != 0 && currentDistance > firstAndSecondHalfDistance and not fourthStage and not fifthStage and goalID == -1) {
                double dA = w[2] * dt;
                //std::vector<double> velocity = {sample->velocity * dir[0], sample->velocity * dir[1], 0};

                if(fromAngle > toAngle) {
                    circulating = LeftCirculating;
                    fromAngle -= dA;
                    shipData->angleVelocity = -w[2];
                    shipData->wIndex = -2;

                    setupDirection(this->fromAngle);
                }

                else if(fromAngle < toAngle) {
                    circulating = RightCirculating;
                    fromAngle += dA;
                    shipData->angleVelocity = w[2];
                    shipData->wIndex = 2;

                    setupDirection(this->fromAngle);
                }

                if(currentDistance + sample->velocity * dt > firstAndSecondDistance) {
                    qDebug() << "end 3 stage";
                    stage = "Первинний пошук";
                    toAngle += teta0;
                    fourthStage = true;
                }

                std::vector<double> velocity = {dir[0] * sample->velocity, dir[1] * sample->velocity, dir[2] * sample->velocity};


                shipData->position[0] = sample->position[0] + velocity[0] * dt;
                shipData->position[1] = sample->position[1] + velocity[1] * dt;
                shipData->position[2] = sample->position[2];

                shipData->velocity = sample->velocity;

                shipData->distanceToGoal = this->distanceToGoal;

                currentDistance += sample->velocity * dt;
                shipData->beta = this->beta;

                shipData->currentDistance = currentDistance;


                TorpedoControllerSampleData * sData = new TorpedoControllerSampleData;

                sData->alpha = this->fromAngle;
                sData->stage = stage;
                //qDebug() << this->fromAngle;

                this->e->getPiSamples()->pushSample(this->piId, sData);

            }

            else if((distanceToTrajectory == 0 or torpedoDivergency == 0) and not fourthStage and not fifthStage and goalID == -1) {
                qDebug(s) << "end 3 stage";
                stage = "Первинний пошук";
                toAngle += teta0;
                fourthStage = true;
                *shipData = *sample;
                shipData->distanceToGoal = this->distanceToGoal;
                shipData->currentDistance = currentDistance;
                shipData->beta = this->beta;


                TorpedoControllerSampleData * sData = new TorpedoControllerSampleData;

                sData->alpha = this->fromAngle;
                sData->stage = stage;
                //qDebug() << this->fromAngle;

                this->e->getPiSamples()->pushSample(this->piId, sData);
            }

            else if((currentDistance > firstAndSecondDistance or distanceToTrajectory == 0) and goalID == -1 and not fifthStage) {

                double dA = w[3] * dt;
                //std::vector<double> velocity = {sample->velocity * dir[0], sample->velocity * dir[1], 0};
                if(fromAngle > toAngle) {
                    circulating = LeftCirculating;
//                    stage = "Ліва циркуляція 30 градусів";
                    fromAngle -= dA;
                    shipData->angleVelocity = -w[3];
                    shipData->wIndex = -3;

                    setupDirection(this->fromAngle);
                    if(fromAngle <= toAngle) {
                        toAngle += 2 * teta0;
                    }
                }

                else if(fromAngle < toAngle) {
                    circulating = RightCirculating;
//                    stage = "Права циркуляція 30 градусів";
                    fromAngle += dA;
                    shipData->angleVelocity = w[3];
                    shipData->wIndex = 3;

                    setupDirection(this->fromAngle);
                    if(fromAngle >= toAngle) {
                        toAngle -= 2 * teta0;
                    }
                }

                std::vector<double> velocity = {dir[0] * sample->velocity, dir[1] * sample->velocity, dir[2] * sample->velocity};


                shipData->position[0] = sample->position[0] + velocity[0] * dt;
                shipData->position[1] = sample->position[1] + velocity[1] * dt;
                shipData->position[2] = sample->position[2];

                shipData->velocity = sample->velocity;

                shipData->distanceToGoal = this->distanceToGoal;

                currentDistance += sample->velocity * dt;

                shipData->currentDistance = currentDistance;
                shipData->beta = this->beta;

                TorpedoControllerSampleData * sData = new TorpedoControllerSampleData;

                sData->alpha = this->fromAngle;
                sData->stage = stage;
                //qDebug() << this->fromAngle;

                this->e->getPiSamples()->pushSample(this->piId, sData);



            }
            else if(goalID != -1 and not fifthStage) {
                auto goal = this->e->getPiSamples()->getLastSample<PhysicalItemSampleData>(goalID);
//                    auto goalAngle = atan2((goal->position[0] - ), goal->position[0]);


                auto dist = KMath::metersDistance(sample->position[0], sample->position[1],
                                                     goal->position[0], goal->position[1]);


                beta = KMath::getAngle(goal->position[0] - sample->position[0], goal->position[1] - sample->position[1]);
//                    auto minBound = KMath::degreesToRadians(sampleC->alpha - 20);
//                    auto maxBound = KMath::degreesToRadians(sampleC->alpha + 20);
//                qDebug(s) << beta << /*dist << minBound << maxBound <<*/ "my position" << sample->position[0] << sample->position[1]
//                        << "object" << goal->position[0] << goal->position[1];


                shipData->distanceToGoal = dist;

                if(dist < 100.) {
                    stage = "Атака";
                }

                double dA = 13. * dt;

                auto signFromAngle = KMath::signPiPeriod(fromAngle);

                if(signFromAngle > beta) {
                    circulating = LeftCirculating;
                    fromAngle -= dA;
                    shipData->angleVelocity = -w[3];
                    shipData->wIndex = -3;

                    setupDirection(this->fromAngle);
                }

                else if(signFromAngle < beta) {
                    circulating = RightCirculating;
                    fromAngle += dA;
                    shipData->angleVelocity = w[3];
                    shipData->wIndex = 3;

                    setupDirection(this->fromAngle);
                }
                if(!KMath::isInBounds(signFromAngle - sector, beta, signFromAngle + sector)) {
                    qDebug(s) << "Lost goal, doing maneure";
                    this->stage = "Маневр";
                    fifthStage = true;
                    goalID = -1;
                    if(signFromAngle < 0) {
                        toAngle = KMath::degreesToRadians(-360);
                    }
                    else toAngle = KMath::degreesToRadians(360);
                    toAngle += signFromAngle;
                    afterLostDistance = 0;
                }
                else {

                    if(dist <= killDistance) {
                        stage = "Ціль знищено";
                        auto goalController = dynamic_cast<BaseMoveController*>(this->e->getPiControllers()[goalID]);
                        goalController->setDestroyed(true);
                        this->setDestroyed(true);
                    }
                }


                std::vector<double> velocity = {dir[0] * sample->velocity, dir[1] * sample->velocity, dir[2] * sample->velocity};


                shipData->position[0] = sample->position[0] + velocity[0] * dt;
                shipData->position[1] = sample->position[1] + velocity[1] * dt;
                shipData->position[2] = sample->position[2];

                shipData->velocity = sample->velocity;

                currentDistance += sample->velocity * dt;

                shipData->currentDistance = currentDistance;
                shipData->beta = this->beta;


                TorpedoControllerSampleData * sData = new TorpedoControllerSampleData;

                sData->alpha = signFromAngle;
                sData->stage = stage;
                //qDebug() << this->fromAngle;

                this->e->getPiSamples()->pushSample(this->piId, sData);

            }
            else if (fifthStage and goalID == -1) {

                if(afterLostDistance <= afterLostMaxDistance) {
//                    double dA = w[0]
                    shipData->angleVelocity = fromAngle > toAngle ? -w[3] : w[3];
                    shipData->wIndex = fromAngle > toAngle ? -3 : 3;
                }
                else {
                    double dA =  w[3] * dt;

                    if(fromAngle > toAngle) {
                        circulating = LeftCirculating;
                        fromAngle -= dA;
                        shipData->angleVelocity = -w[3];
                        shipData->wIndex = -3;

                        setupDirection(this->fromAngle);

                        if(fromAngle <= toAngle) {
                            qDebug(s) << "first search";
                            this->stage = "Первинний пошук";
                            fifthStage = false;
                            fourthStage = true;
                            afterLostDistance = 0;
                            toAngle += teta0;
                        }
                    }

                    else if(fromAngle < toAngle) {
                        circulating = RightCirculating;
                        fromAngle += dA;
                        shipData->angleVelocity = w[3];
                        shipData->wIndex = 3;

                        setupDirection(this->fromAngle);
                        if(fromAngle >= toAngle) {
                            qDebug(s) << "first search";
                            this->stage = "Первинний пошук";
                            toAngle += teta0;
                            fifthStage = false;
                            fourthStage = true;
                            afterLostDistance = 0;
                        }
                    }
                }


                std::vector<double> velocity = {dir[0] * sample->velocity, dir[1] * sample->velocity, dir[2] * sample->velocity};


                shipData->position[0] = sample->position[0] + velocity[0] * dt;
                shipData->position[1] = sample->position[1] + velocity[1] * dt;
                shipData->position[2] = sample->position[2];

                shipData->velocity = sample->velocity;

                shipData->distanceToGoal = this->distanceToGoal;

                currentDistance += sample->velocity * dt;
                afterLostDistance += sample->velocity * dt;

                shipData->currentDistance = currentDistance;
                shipData->beta = this->beta;


                TorpedoControllerSampleData * sData = new TorpedoControllerSampleData;

                sData->alpha = KMath::signPiPeriod(this->fromAngle);
                sData->stage = stage;
                //qDebug() << this->fromAngle;

                this->e->getPiSamples()->pushSample(this->piId, sData);

            }

//            shipData->angleVelocity = circulating ? -;




            //if(getFullDistance() > distanceToTrajectory) {

    //        std::vector<double> velocity;

    //        velocity = {dir[0] * sample->velocity, dir[1] * sample->velocity, dir[2] * sample->velocity};

    ////        if(sample->velocity < goalVelocity) {

    ////            this->velocity += acceleration * dt;

    ////        }

    //        shipData->velocity = sample->velocity;

    //        shipData->position[0] = sample->position[0] + (velocity[0] * dt);
    //        shipData->position[1] = sample->position[1] + (velocity[1] * dt);



    //        if(currentCmd.getParameter("Z")->modifier == "+") {
    //            shipData->position[2] = shipPrevData->position[2] < zGoal ? shipPrevData->position[2] + (velocity[2] * dt) : shipPrevData->position[2];
    //        }
    //        else if(currentCmd.getParameter("Z")->modifier == "-") {
    //            shipData->position[2] = shipPrevData->position[2] > zGoal ? shipPrevData->position[2] - (velocity[2] * dt) : shipPrevData->position[2];
    //        }


    //        currentDistance += sample->velocity * dt;

    //        qDebug() << "alpha" << alpha;

    //        double vy = sample->velocity * dir[1];

    //        qDebug() << "vy" << sample->velocity << vy;

    //        std::vector<double> tAndV = count_next_position(sample->position[0], sample->position[1], sample->position[2], onLife, dt, vy);
    //        sData->position[0] = tAndV[0];
    //        sData->position[1] = tAndV[1];
    //        sData->position[2] = tAndV[2];
            //qDebug() << tAndV << sample->velocity;

           // }

            //qDebug() << sData->position << sData->velocity;


            onLife += dt;
            shipData->onLife = onLife;
            shipData->goalFound = goalID != -1;
            return true;
        }
        else {
            *shipData = *sample;
            shipData->distanceToGoal = this->distanceToGoal;
            shipData->beta = this->beta;
            shipData->wIndex = 0;
            shipData->angleVelocity = 0;
            return true;
        }
    }

    return false;
}

void TorpedoController::proceed(long long uSecond, double dt)
{
    //int dt = uSecond - prvTime;
    TorpedoSampleData * newSampleData = new TorpedoSampleData;

//    auto p = e->piContainer->getLastSample<TorpedoSampleData>(piId);
//    auto p = e->piContainer->getLastSample<TorpedoControllerSampleData>(piId);
    double timeInSeconds = dt / MCS_IN_S;
    newSampleData->uSecond = uSecond;
    if(this->move(newSampleData, timeInSeconds))
        this->e->getPiSamples()->pushSample<TorpedoSampleData>(this->piId, newSampleData);
    else delete newSampleData;
}

void TorpedoController::init(BaseMsg *msg, AbstractEngine *e)
{
    setEngine(e);
    CreatePIMsg * itemData = static_cast<CreatePIMsg*>(msg);
    TorpedoSampleData * tData = new TorpedoSampleData;
    this->e->getPiSamples()->CreateNewRecord(this->piId, tData->getType(), false);
    tData->position[0] = itemData->position[0];
    tData->position[1] = itemData->position[1];
    tData->position[2] = itemData->position[2];
    tData->velocity = itemData->velocity;
    tData->uSecond = 0;
    tData->acceleration = {0, 0, 0};
    this->e->getPiSamples()->pushSample(this->piId, tData);
    TorpedoControllerSampleData * tcData = new TorpedoControllerSampleData;
    tcData->piId = this->piId;
    tcData->uSecond = 0;
    tcData->theta = 0;
    this->alpha = KMath::degreesToRadians(itemData->angle);
    tcData->alpha = this->alpha;
    this->setupDirection(this->alpha);
    this->e->getPiSamples()->CreateNewRecord(this->piId, tcData->getType(), false);
    this->e->getPiSamples()->pushSample(this->piId, tcData);
}

void TorpedoController::setup(double firstRA, double sRA, double torpedoDivergency, double rotateDistance)
{
    this->firstRA = firstRA;
    this->secondRA = sRA;
    this->torpedoDivergency = torpedoDivergency;
    if(rotateDistance > 0 || torpedoDivergency > 0) {
        this->stage = "Вихід на траекторію";
    }
    this->maxLeft = this->alpha - firstRA - this->t_rot;
    this->maxRight = this->maxLeft + 2 * t_rot;
    this->firstAndSecondDistance = distanceToTrajectory + 2 * torpedoDivergency;
    this->firstAndSecondHalfDistance = distanceToTrajectory + torpedoDivergency;
    this->distanceToTrajectory = rotateDistance;
    setupDirection(this->alpha);
//    initStartAngle();
    this->fromAngle = this->alpha;
    this->toAngle = fromAngle + firstRA;
    setParams = true;
    qDebug(s) << "data:" << this->alpha << toAngle << firstRA << secondRA << torpedoDivergency << dir[0] << dir[1] << dir[2] << fromAngle << maxLeft << maxRight;
    //if(toAngle < degreesToRadians(-180)) = degreesToRadians(-180);
}

void TorpedoController::initStartAngle()
{
    fromAngle = KMath::getAngle(dir[0], dir[1]);
}

void TorpedoController::setupDirection(double angle)
{
//    if(angle < 0) {
//        angle += 2 * M_PI;
//    }
    this->dir[0] = sin(angle);
    this->dir[1] = cos(angle);
    this->dir[2] = 0;
}

//void TorpedoController::initStartAngle()
//{
//    if(dir[0] < dir[1]) {
//        fromAngle = atan(dir[0] / dir[1]);
//    }
//    else {
//        fromAngle = M_PI_2 - atan(dir[1] / dir[0]);
//    }

//    if(fromAngle < 0) {
//        fromAngle += 2 * M_PI;
//    }

//    if(dir[0] < 0 && dir[1] < 0) {
//        fromAngle += M_PI;
//    }
//}

//std::vector<double> TorpedoController::count_next_position(double x, double y, double z, double t0, double dt, double vy){
//    double ti = t0;
//    double tk = t0;
//    int nx = 0;
//    int nz = 0;
//    while(tk>=this->t_rot and this->t_rot!=0){
//        nz += 1;
//        tk-=this->t_rot;
//    }
//    double vx = 0.0, vz = 0.0;
//    if(nz % 4 == 0)
//        vz = -1 * vy * tan(this->w * tk);
//    if(nz % 4 == 1)
//        vz = -1 * vy * tan(this->teta0 - this->w * tk);
//    if(nz % 4 == 2)
//        vz = vy * tan(this->w * tk);
//    if(nz % 4 == 3)
//        vz = vy * tan(this->teta0 - this->w * tk);
//    while(ti >= 2 * this->t_rot and this->t_rot != 0){
//        nx += 1;
//        ti -= 2 * this->t_rot;
//    }
//    double cx = pow(-1, nx);
//    vx = cx * vy * tan(this->teta0 - this->w * ti);
//    x += vx * dt;
//    y += vy * dt;
//    z += vz * dt;
//    this->fromAngle = cx * atan(this->teta0 - this->w * ti);

//    if(fromAngle >= maxLeft && fromAngle > (maxLeft + maxRight) / 2) {
//        circulating = RightCirculating;
//        stage = "right circulation 30 degrees";
//    }

//    if(fromAngle <= maxRight && fromAngle < (maxLeft + maxRight) / 2) {
//        circulating = LeftCirculating;
//        stage = "left circulation 30 degrees";
//    }

//    return {x,y,z,vx,vz};
//}

void TorpedoController::proceedAfter(long long uSecond, double dt)
{
    Q_UNUSED(uSecond)
    Q_UNUSED(dt)
    if(goalID == -1) {
        auto samples = this->e->getPiSamples();
        auto torpedoLastData = samples->getLastSample<TorpedoSampleData>(this->piId);
        auto torpedoCLastData = samples->getLastSample<TorpedoControllerSampleData>(this->piId);
        for(auto & object : this->e->getPiControllers()) {
            auto id = object.first;
            if(id != this->piId and object.second->name() == "Vessel") {
                auto sampleData = samples->getLastSample<PhysicalItemSampleData>(id);
                auto dist = KMath::metersDistance(torpedoLastData->position[0], torpedoLastData->position[1],
                                                     sampleData->position[0], sampleData->position[1]);
                this->distanceToGoal = dist;
                this->beta = KMath::getAngle(sampleData->position[0] - torpedoLastData->position[0], sampleData->position[1] - torpedoLastData->position[1]);
                auto minBound = torpedoCLastData->alpha - sector;
                auto maxBound = torpedoCLastData->alpha + sector;
//                qDebug(s) << beta << dist << minBound << maxBound << "my position" << torpedoLastData->position[0] << torpedoLastData->position[1]
//                        << "object" << sampleData->position[0] << sampleData->position[1];
                if(KMath::isInBounds(minBound, this->beta, maxBound) && dist <= minDistanceToAttack) {
                    goalID = id;

                    qDebug(g) << "found goal, id" << id;
                    fifthStage = false;
                    afterLostDistance = 0;
//                    auto torpedoCData = new TorpedoControllerSampleData;
                    this->stage = "Супроводження";
//                    torpedoCData->uSecond = uSecond;
//                    torpedoCData->alpha = torpedoCLastData->alpha;
//                    this->e->getPiSamples()->pushSample(piId, torpedoCData);
                    return;
                }
            }
        }
    }
}

QVariantMap TorpedoController::getInfo()
{
    auto torpedoData = this->e->getPiSamples()->getLastSample<TorpedoSampleData>(piId);
//    auto torpedoCData = this->e->getPiSamples()->getLastSample<TorpedoControllerSampleData>(piId);
    return QVariantMap{
        {"objectType", torpedoData->getType()},
        {"x", torpedoData->position[0]},
        {"y", torpedoData->position[1]},
        {"velocity", torpedoData->velocity},
        {"xDirection", torpedoData->direction[0]},
        {"yDirection", torpedoData->direction[1]},
//        {"rotation", torpedoCData->alpha},
        {"angleVelocity", torpedoData->angleVelocity},
        {"wIndex", torpedoData->wIndex}
    };
}

QString TorpedoController::name()
{
    return "Torpedo";
}

//void TorpedoController::set_main_coord(PhysicalItemSampleData* tData){

//    TorpedoSampleData *torpedoNewData = static_cast<TorpedoSampleData*>(tData);
//    TorpedoSampleData * prevData = this->e->getPiSamples()->getLastSample<TorpedoSampleData>(this->piId);
//    std::vector<double> x = {0};
//    std::vector<double> y = {0};
//    std::vector<double> z = {this->depth};

//    std::vector<double> t(&time[0], &time[time.size() - 1]);

//    double dt = t[1];

//    double vy = calculateModule(prevData->velocity) * cos(this->teta0);
////    std::vector<std::vector<double>> ei;
////    std::vector<std::vector<double>> ej;
////    std::vector<std::vector<double>> ek;
//    for(auto t0 : t){
//        if(this->w * t0 >= this->teta0){
//            this->t_rot = t0;
//            break;
//        }
//    }
//    for (auto t0 : t){
//        std::vector<double> tAndV = this->count_next_position(x[x.size()-1], y[x.size()-1], z[x.size()-1], t0, dt, vy);
//        //xt,yt,zt,vx,vz =
////        double v = sqrt(pow(tAndV[3], 2)+pow(vy, 2)+pow(tAndV[4], 2));
////        double vyx = sqrt(pow(tAndV[3], 2)+pow(vy, 2));
////        ej.push_back(np.array([vx,vy,vz]/v))
////        ei.append(np.array([vy,-vx,0]/vyx))
////        ek.append(np.array([-vx*vz,-vz*vy,vy**2+vx**2])/(v*vyx))
//        x.push_back(tAndV[0]);
//        y.push_back(tAndV[1]);
//        z.push_back(tAndV[2]);
//    }
////    self.e1 = ei
////    self.e2 = ej
////    self.e3 = ek
//    //torpedoNewData->position = pd.DataFrame({'t':self.time, 'x':x, 'y':y,'z':z}) # torpedo position at main time moments
//}

//rotate A+90 R100;
//forward d150;
//rotate a+30 r100;
//forward d500;
//rotate a-30 r100;
//forward d500;
//rotate a+30 r100;
//forward d100;
//rotate a-60 r100;
//forward d100;
//rotate a+60 r100;
//forward d100;
//rotate a-60 r100;
//forward d100;


template<typename T>
std::vector<double> linspace(T start_in, T end_in, int num_in)
{

  std::vector<double> linspaced;

  double start = static_cast<double>(start_in);
  double end = static_cast<double>(end_in);
  double num = static_cast<double>(num_in);

  if (num == 0) { return linspaced; }
  if (num == 1)
    {
      linspaced.push_back(start);
      return linspaced;
    }

  double delta = (end - start) / (num - 1);

  for(int i=0; i < num-1; ++i)
    {
      linspaced.push_back(start + delta * i);
    }
  linspaced.push_back(end); // I want to ensure that start and end
                            // are exactly the same as the input
  return linspaced;
}
