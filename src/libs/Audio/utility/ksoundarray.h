#ifndef KSOUNDARRAY_H
#define KSOUNDARRAY_H

#include "sound.h"
#include <QVector>


//!
//! \brief The KSoundArray class
//!
class AUDIO_EXPORT KSoundArray : public Sound {
    Q_OBJECT
    QMap<int, QList<Sound*>> m_sounds;
protected:
    virtual void overrideAddSound(Sound *){}
public:
    KSoundArray(bool play = false, QObject *parent = nullptr);
    void addSound(int id, QVector<Sound *> *vectorSound);
    void addSound(int id, Sound *sound);
    int size();
    void removeSound(int id, Sound *sound);
    void removeSound(int id, QString type);
    QList<Sound *> sounds() const;
signals:
    void soundAdded(Sound *sound);
    void soundRemoved(Sound *sound);
};

#endif // KSOUNDARRAY_H
