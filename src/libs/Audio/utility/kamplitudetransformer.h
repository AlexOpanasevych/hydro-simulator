#ifndef KAMPLITUDETRANSFORMER_H
#define KAMPLITUDETRANSFORMER_H

#include "../sound.h"

#include <QElapsedTimer>
#include <functional>

class AUDIO_EXPORT KAmplitudeTransformer : public Sound {
    Sound *m_child = nullptr;
    std::function<double(qint64)> m_functinon;
    QElapsedTimer elapsedFromStart;
    qint64 m_interval;
public:
    KAmplitudeTransformer(bool play, QObject *parent);
    Sound *child() const;
    void setChild(Sound *child);
    std::function<double (qint64)> functinon() const;
    void setFunctinon(const std::function<double (qint64)> &functinon);
    qint64 interval() const;
    void setInterval(const qint64 &interval);
    void reset();

    // KSound interface
protected:
    double signal(qint64 index) override;
};

#endif // KAMPLITUDETRANSFORMER_H
