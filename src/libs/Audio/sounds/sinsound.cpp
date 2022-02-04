#include "sinsound.h"
#include <QtMath>
#include <math/kmath.h>


//!
//! \brief Constructor for SinSound class
//! \param play
//! \param parent
//!
SinSound::SinSound(bool play, QObject *parent) : Sound(play, parent), integrator(0.9)
{
    m_oscillator = [](double x){ return qFastSin(x); };
    m_currentOb++;
    qDebug() << "sin sound created";
    connect(&timer, &QTimer::timeout, this, [this](){
        qDebug() << count << el.elapsed();
        count = 0;
        el.restart();
    });
    timer.start(1000);
    el.start();
}

SinSound::~SinSound()
{
    m_currentOb--;
}

//!
//! \brief Get oscillator function callback
//! \return oscillator callback
//!
std::function<double (double)> SinSound::oscillator() const
{
    return m_oscillator;
}

//!
//! \brief Set oscillator callback
//! \param oscillator
//!
void SinSound::setOscillator(const std::function<double (double)> &oscillator)
{
    m_oscillator = oscillator;
}

void SinSound::setSmoothFun(std::function<double(double)> smoothFun)
{
    m_smoothFun = smoothFun;
}

void SinSound::smoothFun()
{
    if(m_smoothFun == nullptr)
    {
        return;
    }
    else
    {
        setAmplitude(position / 50);
    }
}

//int SinSound::currentOb()
//{
//    return m_currentOb;
//}

//!
//! \brief Overloaded function that generates next 8 bytes block of audio data, which is sin wave, specified by
//!
//! \return Returns 8 bytes block in double form
//!
double SinSound::signal(qint64 index)
{
    count++;

    double value = m_oscillator(2 * M_PI * frequency() * (position++ % maxPosition) / sampleRate() + phase());

//    if(qFuzzyCompare(frequency(), 0))  qDebug() << value;
//    else qDebug() << "now not null" << value
    if ((position >= interval() * sampleRate() / 1000) && (interval() != 0))
    {
        setPlay(false);
        position = 0;
        return value;
    }
    return value;
}

//!
//! \brief Helper function for plugin using
//! \param params
//!
void SinSound::init(QVariantMap params)
{
    Sound::init(params);
}


