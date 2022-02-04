#include "kintegratedsound.h"

#include <QVariant>

Sound *KIntegratedSound::child() const {
    return m_child;
}

void KIntegratedSound::setChild(Sound *child) {
    m_child = child;
    child->setParent(this);
}

QVector<double> KIntegratedSound::amplitudes() const {
    return m_amplitudes;
}

void KIntegratedSound::setAmplitudes(const QVector<double> &amplitudes) {
    m_amplitudes = KMath::instance()->smooth(amplitudes, 0.5, 10);
}

void KIntegratedSound::setAmplitudes(const QVariantList &amplitudes) {
    QVector<double>amplitudeVector(amplitudes.size());
    int i = 0;
    for(auto && a : amplitudes) {
        amplitudeVector[i++] = a.toDouble();
    }
    setAmplitudes(amplitudeVector);
}

qint64 KIntegratedSound::interval() const {
    return m_interval;
}

void KIntegratedSound::setInterval(const qint64 &interval) {
    m_interval = interval;
}

int KIntegratedSound::discretizeCount() const {
    return m_discretizeCount;
}

void KIntegratedSound::setDiscretizeCount(int discretizeCount) {
    m_discretizeCount = discretizeCount;
}

KIntegratedSound::KIntegratedSound(bool play, QObject *parent) : Sound(play, parent) {
    connect(this, &Sound::playChanged, this, [this](bool play){
        if(play) {
            elapsedFromStart.restart();
        }
    });
    if(play) {
        elapsedFromStart.restart();
    }
}

void KIntegratedSound::reset() {
    elapsedFromStart.restart();
}

double KIntegratedSound::signal(qint64 index) {
    if(interval() > 0) {
        qint64 time = elapsedFromStart.elapsed();
        int current = static_cast<int>(KMath::instance()->dmap(time, 0, m_interval, 0, m_amplitudes.size() - 1));
        if(current < m_amplitudes.size() && current >= 0) {
            return child()->volume(index) * ((m_amplitudes.size() > 0) ? m_amplitudes[current] : 0);
        }
    }
    return 0;
}
