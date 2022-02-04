#include "pauseelapsedtimer.h"


//!
//! \brief PauseElapsedTimer::isPaused
//! \return
//!
bool PauseElapsedTimer::isPaused()
{
    return state == TimerState::PAUSED;
}

PauseElapsedTimer::PauseElapsedTimer()
{

}

//!
//! \brief PauseElapsedTimer::pause
//! \return
//!
qint64 PauseElapsedTimer::pause() {
    if(state == TimerState::RUNNING) {
        elapsed += QElapsedTimer::elapsed();
        //elapsedPaused = 0;
        state = TimerState::PAUSED;
    }
    return elapsed;
}

//!
//! \brief PauseElapsedTimer::resume
//!
void PauseElapsedTimer::resume()
{
    if(isPaused()) {
        state = TimerState::RUNNING;
        QElapsedTimer::restart();
    }
}

//!
//! \brief PauseElapsedTimer::startOrRestart
//!
void PauseElapsedTimer::startOrRestart()
{
    switch(state) {
    case TimerState::RUNNING: {
        elapsed = 0;
        QElapsedTimer::restart();
        break;
    }
    case TimerState::STOPPED: {
        state = TimerState::RUNNING;
        isValid() ?
            (void)QElapsedTimer::restart() : QElapsedTimer::start();
        break;
    }
    case TimerState::PAUSED: {
        QElapsedTimer::restart();
        state = TimerState::RUNNING;
        break;
    }
    }
}

//!
//! \brief PauseElapsedTimer::elapsedAll
//! \return
//!
qint64 PauseElapsedTimer::elapsedAll()
{
    if(!isPaused()) {
        elapsed += QElapsedTimer::restart();
    }
    return elapsed;
}

//!
//! \brief PauseElapsedTimer::stop
//!
void PauseElapsedTimer::stop()
{
    state = TimerState::STOPPED;
    elapsed = 0;
}
