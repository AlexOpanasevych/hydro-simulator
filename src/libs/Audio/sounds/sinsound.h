#ifndef SINSOUND_H
#define SINSOUND_H

#include <sound.h>
#include <limits>
#include <functional>
#include <kmacro.h>
#include <QTimer>
#include <QElapsedTimer>
#include <utility/kintegrator.h>

class AUDIO_EXPORT SinSound : public Sound
{
    Q_OBJECT
    static inline int m_currentOb = 0;

private:

    void   checkPosition();
//    K_AUTO_PROPERTY(double, maxPosition, maxPosition, setMaxPosition, maxPositionChanged, 0)
    quint32 maxPosition = std::numeric_limits<quint32>::max();
    quint32 position = 0;
    std::function<double(double)> m_oscillator;

    std::function<double(double)> m_smoothFun = nullptr;

public:
    SinSound(bool play = false, QObject *parent = nullptr);
    ~SinSound();
    double signal(qint64 index) override;
    void setPlaySin(bool play);
    void init(QVariantMap params) override;

    void setSmoothFun(std::function<double(double)> smoothFun);
    void smoothFun();

    std::function<double (double)> oscillator() const;
    void setOscillator(const std::function<double (double)> &oscillator);
    KIntegrator integrator;
    int count = 0;
    QTimer timer;
    QElapsedTimer el;
};

#endif // SINSOUND_H
