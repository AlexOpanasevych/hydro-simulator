#ifndef KINTEGRATEDSOUND_H
#define KINTEGRATEDSOUND_H

#include "../sound.h"

#include <QElapsedTimer>
#include <QVector>

#include <math/kmath.h>


class AUDIO_EXPORT KIntegratedSound : public Sound {
    Q_OBJECT
    Sound *m_child;
    QVector<double> m_amplitudes;
    QElapsedTimer elapsedFromStart;
    qint64 m_interval = 0;
    int m_discretizeCount = 0;
//    KMath *math = KMath::instance();
public:
    KIntegratedSound(bool play = false, QObject *parent = nullptr);
    void reset();

    // KSound interface
    Sound *child() const;
    void setChild(Sound *child);

    QVector<double> amplitudes() const;
    void setAmplitudes(const QVector<double> &amplitudes);
    void setAmplitudes(const QVariantList &amplitudes);

    qint64 interval() const;
    void setInterval(const qint64 &interval);

    int discretizeCount() const;
    void setDiscretizeCount(int discretizeCount);

protected:
    double signal(qint64 index) override;
};

#endif // KINTEGRATEDSOUND_H
