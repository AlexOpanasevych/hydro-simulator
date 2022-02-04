#ifndef AFSOUND_H
#define AFSOUND_H

#include <QObject>
#include "../sound.h"
#include <functional>


class AUDIO_EXPORT AFSound : public Sound
{
    Q_OBJECT
private:

    std::function<double(double t)>m_formFunction = nullptr;

public:
    explicit AFSound(QObject *parent = nullptr);
    void setForm(std::function<double(double t)>formFunc);

signals:

public slots:

    // KSound interface
protected:
    double signal(qint64 index) override;
};

#endif // AFSOUND_H
