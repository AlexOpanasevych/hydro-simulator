#ifndef RAWSOUND_H
#define RAWSOUND_H

#include <sound.h>
#include <QVector>
#include <QMap>

class AUDIO_EXPORT RawSound : public Sound
{
    Q_OBJECT

    static inline int m_countOb = 0;
    static inline QMap<QString, QVector<qint16>> raws;
private:
    double m_interpolationFactor = 1;
    double position = 0;
    QString m_nameRaw = nullptr;
    qint32 m_interval = 0;


    double checkPosition();
    void setNameRaw(const QString &nameRaw);

public:
    RawSound(QString nameFileSound = "", QObject *parent=nullptr, const bool m_play=false);
//    RawSound(QObj)
    ~RawSound();
    bool open(QString nameFileSound);
    QString nameRaw() const;
    int size();

    double interpolationFactor() const;
    void setInterpolationFactor(double interpolationFactor);
    void init(QVariantMap params) override;

    //void setPlayRaw(bool play);

    qint32 interval() const;
    void setInterval(const qint32 &interval);

    static int countOb();

protected:
    virtual double signal(qint64 index = -1) override;
};




#endif // RAWSOUND_H
