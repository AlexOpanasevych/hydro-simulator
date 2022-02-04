#ifndef BK_H
#define BK_H

#include "base/KBus.h"
#include "gsnworker.h"
#include "motordriverworker.h"
#include "novworker.h"

#include <gui/kbusconfigurationwindow.h>
#include <QLayout>
#include <QQuickItem>
#include <kmacro.h>
#include <abstractengine.h>
#include <qfuture.h>
#include <base/global_messages/setmodeenginemsg.h>

class BK : public QObject
{
    Q_OBJECT
    K_SINGLETON(BK)
    K_QML_SINGLETON(BK)

public:
    explicit BK(QObject *parent = nullptr);
    ~BK() override;

    void setEngine(AbstractEngine *engine);

public slots:
    void openWindow();
//    void reset();
signals:
    void setEnginePause();
    void setEngineStart();

private:
    QWindow * window = nullptr;

    KBus * gsnBus = nullptr;
    GSNWorker * gsnWorker = nullptr;
    KBusConfigurationWindow * configGsnBusWidget = nullptr;

    KBus * driverBus = nullptr;
    MotorDriverWorker * driverWorker = nullptr;
    KBusConfigurationWindow * configDriverBusWidget = nullptr;


    KBus * novBus = nullptr;
    NOVWorker * novWorker = nullptr;
    KBusConfigurationWindow * configNovBusWidget = nullptr;

    QWidget * busWidgetsContainer = nullptr;
    QHBoxLayout * busWidgetsLayout = nullptr;

    bool running = false;
    void run();
    AbstractEngine * m_engine = nullptr;

    int w[4] = {0, 4, 7, 11};
    QFuture<void> f;

    unsigned long long cyclePeriod = 1600000;
    int soundVelocity = 1481;

    bool reluFlag = true;

    void setEngineMode(EngineMode mode);

    static QMutex m;
};

#endif // BK_H
