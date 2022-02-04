#ifndef AUDIOMODULE_H
#define AUDIOMODULE_H

#include <QQuickItem>
#include <kmacro.h>
#include <audio.h>

class AudioModule : public QObject
{
    Q_OBJECT
    K_SINGLETON(AudioModule)
    K_QML_SINGLETON(AudioModule)
    K_AUTO_PROPERTY(Audio*, audioService, audioService, setAudioService, audioServiceChanged, new Audio())
public:
    explicit AudioModule(QObject * parent = nullptr);
    ~AudioModule();
};

#endif // AUDIOMODULE_H
