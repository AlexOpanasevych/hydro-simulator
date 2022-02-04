#include "engine.h"
#include <QtConcurrent>
#include <chrono>
#include <ctime>
#include <libs/core/engineinterface.h>
#include <libs/PluginsBase/factories/controllerfactory.h>
#include <libs/core/basemovecontroller.h>
#include <cdebug.h>

using namespace std::chrono_literals;

QMutex Engine::m;

Engine::Engine(QObject *parent) : QObject(parent)
{
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [this](){
        m_interface->proceed();
    });

    lastCreatedPIID = 0;

    //installMsgHandler<CreatePIMsg, CreatePIAnsw>([](CreatePIMsg msg) -> CreatePIMsgAns {
    //    Message answ;
    //
    //    return answ;
    //});

    this->messageDispatcher->registerMsgHandler(&Engine::proceedCreateItemMsg, this);
    this->messageDispatcher->registerMsgHandler(&Engine::proceedRemoveItemMsg, this);
    this->messageDispatcher->registerMsgHandler(&Engine::proceedSetEngineModeMsg, this);

//    factory.registerController("Vessel", [](){
//        return new VesselController;
//    });

//    factory.registerController("Torpedo", [](){
//        return new VesselController;
//    });
}

Engine::~Engine()
{
    future.cancel();
    future.waitForFinished();
}

void Engine::start()
{
    future = QtConcurrent::run([this](){
        long long timeError = 0;
        long long iter = 0;
        const int delay = 1000; // microsec
//        auto start = std::chrono::system_clock::now();
        auto last = std::chrono::system_clock::now();
        while (this->working) {
            auto now = std::chrono::system_clock::now();

            auto dt = std::chrono::duration_cast<std::chrono::microseconds>(now - last).count() + timeError;
            if (dt >= delay) {
                timeError = dt - delay;
//                auto t = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count();
//                auto startProceed = std::chrono::system_clock::now();
                this->proceed(iter++ * delay, delay);
//                auto proceedDt = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - startProceed).count();

//                qDebug(k) << "Proceed dt: " << proceedDt;

//                auto proceedDt = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - start).count();
//                qDebug() << "dt" << dt << "t" << t << "after" << proceedDt << proceedDt - t;
                last = now;  //std::chrono::system_clock::now();
//                QThread::usleep(1000);
            }
        }

    });

    m_timer->start(100);
}

void Engine::stop()
{
    working = false;
    //this->piSamples->working = false;
    //this->piSamples->future.cancel();
    future.waitForFinished();
}

CreatePIMsgAns Engine::proceedCreateItemMsg(CreatePIMsg msg)
{
    QString type = msg.typeOfObject;
    CreatePIMsgAns answ;
    if (createbleTypes.contains(msg.typeOfObject))
    {
        AbstractPIController *controller = controllers[type]->create();
        controller->setPiId(this->lastCreatedPIID);
        qDebug(v) << msg.typeOfObject << lastCreatedPIID;
        controller->init(&msg, this);

        this->insertController(this->lastCreatedPIID, controller);
        answ.createdId = this->lastCreatedPIID;
        lastCreatedPIID++;
    }
    else {
        answ.createdId = -1;
        qDebug(v)  << "Engine Error: can't find controller for " << msg.typeOfObject;
    }
    return answ;
}

RemoveItemMessageAns Engine::proceedRemoveItemMsg(RemoveItemMessage msg)
{
    RemoveItemMessageAns ans;
    for (int id : msg.ids) {
        if(!this->removeController(id)) {
            ans.removed = false;
            return ans;
        }
    }
    ans.removed = true;
    return ans;
}

SetModeEngineMsgAns Engine::proceedSetEngineModeMsg(SetModeEngineMsg msg)
{
    if(msg.mode == EngineMode::START)
        this->doMath = true;
    if(msg.mode == EngineMode::PAUSE || msg.mode == EngineMode::RESET)
        this->doMath = false;
    if(msg.mode == EngineMode::RESET) {
        for (auto p : m_piControllers) {
            delete p.second;
        }
        this->m_piControllers.clear();
//        this->lastCreatedPIID = 0;
        m.lock();
        this->getPiSamples()->clear();
        m.unlock();
    }
    SetModeEngineMsgAns msgAnswer;
    msgAnswer.modeChangedSuccess = true;
    msgAnswer.mode = msg.mode;
    return msgAnswer;
}

void Engine::setCreatebleTypes(const QStringList &value)
{
    createbleTypes = value;
}

void Engine::setControllers(const QHash<QString, ControllerFactory *> &value)
{
    controllers = value;
}

// and more msgHandlers
