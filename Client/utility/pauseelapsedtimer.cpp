#include "pauseelapsedtimer.h"

bool PauseElapsedTimer::isPaused()
{
    return state == TimerState::PAUSED;
}

PauseElapsedTimer::PauseElapsedTimer()
{

}

quint64 PauseElapsedTimer::pause() {
    if(state == TimerState::RUNNING) {
        elapsed += QElapsedTimer::elapsed();
        //elapsedPaused = 0;
        state = TimerState::PAUSED;
    }
    return elapsed;
}

void PauseElapsedTimer::resume()
{
    if(isPaused()) {
        state = TimerState::RUNNING;
        QElapsedTimer::restart();
    }
}

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

quint64 PauseElapsedTimer::elapsedAll()
{
    if(!isPaused()) {
        elapsed += QElapsedTimer::restart();
    }
    return elapsed;
}

void PauseElapsedTimer::stop()
{
    state = TimerState::STOPPED;
    elapsed = 0;
}
