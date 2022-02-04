#ifndef KSCENEPLAYER_H
#define KSCENEPLAYER_H

#include <QElapsedTimer>
#include <QObject>
#include <libs/klibcorelite/kmacro.h>
#include <libs/core/engineinterface.h>

class KScenePlayer : public QObject
{
public:
    explicit KScenePlayer(QObject *parent = nullptr);
    enum PlayerMode{
        PLAY    = 0x00, // playing mode
        PAUSE   = 0x01,    // pause mode
        STOP    = 0x02    // reset
    };
private:
    Q_OBJECT
    K_QML_TYPE(KScenePlayer)
    K_READONLY_PROPERTY(qint64, currentTime, currenttime, setCurrenttime, currentTimeChanged, 0)
    K_AUTO_PROPERTY(int, maxTime, maxTime, setMaxTime, maxTimeChanged, 0)
    K_AUTO_PROPERTY(double, speed, speed, setSpeed, speedChanged, 1)
    K_READONLY_PROPERTY(PlayerMode, mode, mode, setMode, modeChanged, PlayerMode::STOP)
    K_READONLY_PROPERTY(PlayerMode, prevMode, prevMode, setPrevMode, prevModeChanged, PlayerMode::STOP)

    Q_ENUM(PlayerMode)
public slots:
    void start();
    void stop();
    void pause();
    void updateTime();
signals:
    void startSignal();
    void pauseSignal();
    void stopSignal();
private:
    QElapsedTimer timer;
    qint64 oldTime = 0;
};

#endif // KSCENEPLAYER_H
