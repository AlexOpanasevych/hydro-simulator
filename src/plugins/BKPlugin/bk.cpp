#include "bk.h"

#include <kbusfactory.h>
#include <gsnworker.h>
#include <cdebug.h>

#include <QWindow>
#include <QThread>
#include <QtConcurrent>
#include <physicalitemsampledata.h>
#include <math.h>
#include <QApplication>

QMutex BK::m;

BK::BK(QObject *parent)
    : QObject(parent) {

    busWidgetsContainer = new QWidget();
    busWidgetsLayout = new QHBoxLayout();

    busWidgetsContainer->setLayout(busWidgetsLayout);

    gsnWorker = new GSNWorker(this);
    driverWorker = new MotorDriverWorker();
    novWorker = new NOVWorker();

    auto connectLambda = [](KBus **bus, QVariantMap data, KBusWorker * worker){
        if (*bus) {
            delete *bus;
            *bus = nullptr;
        }

        *bus = KBusFactory::instance()->create("KSerialBusThreaded", data);
        (*bus)->start();
        (*bus)->addWorker(worker);

        QThread::msleep(100);

        if((*bus)->isConnected()){
            qDebug() << "Port opened!";
//            configGsnBusWidget->close();
        } else {
            qDebug() << "Failed to open port!";
        }

//        setEngineMode(EngineMode::PAUSE);
    };

    auto disconnectLambda = [](KBus **bus){
        if(*bus) delete *bus;
        *bus = nullptr;
        qDebug() << "Port closed!";

//        setEngineMode(EngineMode::START);
    };

    // GSN Bus -----------------------------------------------------------------------------------------------------------

    configGsnBusWidget = new KBusConfigurationWindow("GSN Bus", "KSerialBusThreaded", nullptr);
    connect(configGsnBusWidget, &KBusConfigurationWindow::connectButtonClicked, [this, connectLambda](QVariantMap data) {
        connectLambda(&gsnBus, data, gsnWorker);
    });

    connect(configGsnBusWidget, &KBusConfigurationWindow::disconnectButtonClicked, std::bind(disconnectLambda, &gsnBus));

    // Driver Bus -----------------------------------------------------------------------------------------------------------

    configDriverBusWidget = new KBusConfigurationWindow("Driver Bus", "KSerialBusThreaded", nullptr);
    connect(configDriverBusWidget, &KBusConfigurationWindow::connectButtonClicked, [this, connectLambda](QVariantMap data) {
        connectLambda(&driverBus, data, driverWorker);
    });

    connect(configDriverBusWidget, &KBusConfigurationWindow::disconnectButtonClicked, std::bind(disconnectLambda, &driverBus));

    // NOV Bus -----------------------------------------------------------------------------------------------------------

    configNovBusWidget = new KBusConfigurationWindow("NOV Bus", "KSerialBusThreaded", nullptr);
    connect(configNovBusWidget, &KBusConfigurationWindow::connectButtonClicked, [this, connectLambda](QVariantMap data) {
        connectLambda(&novBus, data, novWorker);
        novWorker->initNov();
    });

    connect(configNovBusWidget, &KBusConfigurationWindow::disconnectButtonClicked, std::bind(disconnectLambda, &driverBus));


    busWidgetsLayout->addWidget(configGsnBusWidget);
    busWidgetsLayout->addWidget(configDriverBusWidget);
    busWidgetsLayout->addWidget(configNovBusWidget);

    run();
}

BK::~BK()
{
    running = false;
    f.waitForFinished();
}

void BK::openWindow()
{
    busWidgetsContainer->show();
}

void BK::setEngineMode(EngineMode mode)
{
    switch(mode){
    case EngineMode::PAUSE:
        emit setEnginePause();
        break;
    case EngineMode::START:
        emit setEngineStart();
        break;
    default:
        break;
    }
}

void BK::run(){
    running = true;

    if(/*head link checking*/false){
        setEngineMode(EngineMode::PAUSE);
    }

    f = QtConcurrent::run([this](){
        bool stopFlag = true;

        QAtomicInteger<unsigned long long> lastTimeUsec = 0;

        while (running) {
            if(m_engine->doMath){
                stopFlag = false;

                if (m_engine->m_engineTime - lastTimeUsec >= cyclePeriod){
                    novWorker->startOnceBlink();
//                    qDebug(k) << "Send command bk";

                    auto controllers = std::ref(m_engine->getPiControllers());
                    auto search = std::find_if(controllers.get().begin(), controllers.get().end(), [](const std::pair<int, AbstractPIController*> & c){
                        return "Torpedo" == c.second->name();
                    });

                    if(search != controllers.get().end()) {
                        auto sample = m_engine->getPiSamples()->getLastSample<PhysicalItemSampleData>((*search).first);
                        QVariantMap torpedoInfo;
                        sample->getAsVariant(torpedoInfo);
                        int wInd = torpedoInfo["wIndex"].toInt();
                        double distanceToGoal = torpedoInfo["distanceToGoal"].toDouble();
                        if(distanceToGoal > 0){
                            if(distanceToGoal <= 100){
                                cyclePeriod = 2 * 100. / soundVelocity * 1000000;
                            }
                            else{
                                cyclePeriod = 2 * distanceToGoal / soundVelocity * 1000000;
                            }

//                            qDebug(k) << "cyclePeriod" << cyclePeriod;
                        }
                        if(wInd == 0) {
                            qDebug(k) << "Send command bk0";
                            gsnWorker->setNoCirculation();
                            gsnWorker->sendCmd();

                            driverWorker->sendAngleVelocity(0);
                        }
                        else{
                            if(wInd < 0) {
//                                qDebug(k) << "Send command bk1";
                                gsnWorker->setLeftCirculation();
                                gsnWorker->setSpeed(-wInd);
                                gsnWorker->sendCmd();

                                driverWorker->sendAngleVelocity(-w[-wInd]);
                            }
                            else{
//                                qDebug(k) << "Send command bk2";
                                gsnWorker->setRightCirculation();
                                gsnWorker->setSpeed(wInd);
                                gsnWorker->sendCmd();

                                driverWorker->sendAngleVelocity(w[wInd]);
                            }
                        }

                        if(reluFlag) gsnWorker->setTopCirculation();
                        else gsnWorker->setBottomCirculation();

                        reluFlag = !reluFlag;
                        lastTimeUsec = m_engine->m_engineTime;
                    }
                    else {
//                        qDebug(k) << "Send command bk0";
                            gsnWorker->setNoCirculation();

                        driverWorker->sendAngleVelocity(0);
                    }
                }
            }
            else{
                if(!stopFlag){
                    driverWorker->sendAngleVelocity(0);
                    cyclePeriod = 1600000;
                    stopFlag = true;
                }
            }

            QThread::msleep(5);
        }

    });
}

void BK::setEngine(AbstractEngine *engine)
{
    m_engine = engine;
}
