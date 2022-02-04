#include "ksceneplayer.h"

#include <QDateTime>

KScenePlayer::KScenePlayer(QObject *parent) : QObject(parent)
{

}

void KScenePlayer::start()
{
    if (mode() != PlayerMode::PLAY)
    {
        timer.restart();
        setPrevMode(mode());
        setMode(PlayerMode::PLAY);
        emit startSignal();
    }
}

void KScenePlayer::stop()
{
    if (mode() != PlayerMode::STOP)
    {
        setCurrenttime(0);
        oldTime = 0;
        setPrevMode(mode());
        setMode(PlayerMode::STOP);
        emit stopSignal();
    }
}

void KScenePlayer::pause()
{
    if (mode() != PlayerMode::STOP)
    {
        qint64 t1 = timer.elapsed();
        if (t1 >= 1000)
        oldTime = t1 + oldTime;
        setPrevMode(mode());
        setMode(PlayerMode::PAUSE);
        emit pauseSignal();
    }
}

void KScenePlayer::updateTime()
{
    if (mode() == PlayerMode::PLAY)
        setCurrenttime((oldTime + timer.elapsed()) / 1000);
}

