#ifndef KSOUNDMIX_H
#define KSOUNDMIX_H

#include "ksoundarray.h"

//!
//! \brief The KSoundMix class
//!
class AUDIO_EXPORT KSoundMix : public KSoundArray {
    Q_OBJECT
    bool m_average = true;
public:
    KSoundMix(bool play = false, QObject *parent = nullptr);

    // KSound interface
    bool average() const;
    void setAverage(bool average);
    void overrideAddSound(Sound * sound) override;

protected:
    double signal(qint64 index) override;
};

#endif // KSOUNDMIX_H
