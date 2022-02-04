#ifndef PAUSEELAPSEDTIMER_H
#define PAUSEELAPSEDTIMER_H

#include <QElapsedTimer>
//#include <QObject>
//#include <QTimer>



class PauseElapsedTimer : public QElapsedTimer
{
//    Q_OBJECT

    enum class TimerState {RUNNING, PAUSED, STOPPED};

    TimerState state = TimerState::STOPPED;

    quint64 elapsed = 0;

    bool isPaused();
public:
    explicit PauseElapsedTimer();

    quint64 pause();
    void resume();
    void startOrRestart();
    quint64 elapsedAll();
    void stop();
//signals:

//    void started();
//    void stopped();
//    void paused();

};

#endif // PAUSEELAPSEDTIMER_H
