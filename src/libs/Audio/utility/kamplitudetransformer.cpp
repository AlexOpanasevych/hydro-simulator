#include "kamplitudetransformer.h"

Sound *KAmplitudeTransformer::child() const {
    return m_child;
}

void KAmplitudeTransformer::setChild(Sound *child) {
    m_child = child;
    child->setParent(this);
}

std::function<double (qint64)> KAmplitudeTransformer::functinon() const {
    return m_functinon;
}

void KAmplitudeTransformer::setFunctinon(const std::function<double (qint64)> &functinon) {
    m_functinon = functinon;
}

qint64 KAmplitudeTransformer::interval() const {
    return m_interval;
}

void KAmplitudeTransformer::setInterval(const qint64 &interval) {
    m_interval = interval;
}

void KAmplitudeTransformer::reset() {
    elapsedFromStart.restart();
}

KAmplitudeTransformer::KAmplitudeTransformer(bool play, QObject *parent) : Sound(play, parent) {
    connect(this, &Sound::playChanged, this, [this](bool play){
        if(play) {
            elapsedFromStart.restart();
        }
    });
}

double KAmplitudeTransformer::signal(qint64 index) {
    if(child() && functinon()) {
        return child()->volume(index) * functinon()(elapsedFromStart.elapsed());
    }
    return 0;
}
