#ifndef PAUSEELAPSEDTIMER_H
#define PAUSEELAPSEDTIMER_H

#include <QElapsedTimer>
#include "../Audio_global.h"
//#include <QObject>
//#include <QTimer>

//!
//! \brief The PauseElapsedTimer class
//!
class AUDIO_EXPORT PauseElapsedTimer : public QElapsedTimer
{
//    Q_OBJECT

    enum class TimerState {RUNNING, PAUSED, STOPPED};

    TimerState state = TimerState::STOPPED;

    qint64 elapsed = 0;

    bool isPaused();
public:
    explicit PauseElapsedTimer();

    qint64 pause();
    void resume();
    void startOrRestart();
    qint64 elapsedAll();
    void stop();
//signals:

//    void started();
//    void stopped();
//    void paused();

};

#endif // PAUSEELAPSEDTIMER_H
