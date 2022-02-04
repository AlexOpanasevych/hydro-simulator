#include "audio.h"
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <sounds/afsound.h>
#include <sounds/kintegratedsound.h>
#include <sounds/rawsound.h>
#include <sounds/sinsound.h>
#include <sounds/soundfactory.h>
#include <utility/ksoundmix.h>

//!
//! \brief Audio::Audio
//! \param parent
//!
Audio::Audio(QObject *parent) : QObject(parent)
{

    SoundFactory::registerSound<KSoundMix>("mix");
    SoundFactory::registerSound<SinSound>("sin");
    SoundFactory::registerSound<RawSound>("raw");
    SoundFactory::registerSound<AFSound>("af");
    SoundFactory::registerSound<KIntegratedSound>("integrated");

    initializeAudio(QAudioDeviceInfo::defaultOutputDevice());
    connect(this, &Audio::volumeChanged, this, &Audio::changeVolume);

}

Audio::~Audio()
{
//    delete audioDevice;
#ifdef Q_WIN_LOOPBACK
    m_loopback->deleteLater();
#endif
}

//!
//! \brief Audio::audioOutput
//! \return
//!
QAudioOutput *Audio::audioOutput() const
{
    return m_audioOutput;
}

//!
//! \brief Audio::addSound
//! \param id
//! \param name
//! \param params
//!
void Audio::addSound(int id, QString name, QVariantMap params)
{
    auto sound = SoundFactory::createSound(name);
    sound->init(params);
    dynamic_cast<KSoundMix*>(device()->rootSound())->addSound(id, sound);
}

//!
//! \brief Audio::stopSound
//! \param id
//! \param name
//!
void Audio::stopSound(int id, QString name)
{
    dynamic_cast<KSoundMix*>(device()->rootSound())->removeSound(id, name);
}

//!
//! \brief Audio::initializeAudio
//! \param deviceInfo
//!
void Audio::initializeAudio(const QAudioDeviceInfo &deviceInfo)
{
    QAudioFormat format;
    format.setSampleRate(48000);
    format.setChannelCount(1);
    format.setSampleSize(16);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::SignedInt);

    if (!deviceInfo.isFormatSupported(format)) {
        qWarning() << "Default format not supported - trying to use nearest";
        format = deviceInfo.nearestFormat(format);
    }

    m_audio_lib = new AudioStreamingLibCore(this);
    m_audio_lib->changeInputDevice(QAudioDeviceInfo::defaultInputDevice());
    AudioStreamingLibInfo info;

    info.setWorkMode(AudioStreamingLibInfo::StreamingWorkMode::BroadcastServer);
    info.setEncryptionEnabled(false);
    info.setGetAudioEnabled(true);
    info.setListenAudioInputEnabled(false);
    info.setNegotiationString("audioStream");
//    info.setOutputDeviceInfo(deviceInfo);
//    info.setID("audioStream");
//    info.setTimeToBuffer(0);
//    info.setAudioFormat(format);
//    info.setOutputDeviceType(AudioStreamingLibInfo::AudioDeviceType::CustomAudioDevice);

#ifdef Q_OS_WIN
    m_loopback = new QWinLoopback(this);
    connect(m_loopback, &QObject::destroyed, this, [=]()
    {
        m_buffer.clear();
    });

    bool started = m_loopback->start();
    if (!started)
        throw std::runtime_error("loopback cannot start!");


    info.setInputDeviceType(AudioStreamingLibInfo::AudioDeviceType::CustomAudioDevice);

    info.setCallBackEnabled(true);

    connect(m_loopback, &QWinLoopback::readyRead, this, [this](const QByteArray &data){
        if (!m_loopback)
            return;

        m_buffer.append(data);
    });

    info.setInputAudioFormat(m_loopback->format());
#else
    // here will be linux implementation of loopback
//    Q_UNUSED(data)

#endif
    connect(m_audio_lib, &AudioStreamingLibCore::inputData, this, [this](const QByteArray &data){
        if (!m_audio_lib || !m_audio_lib->isRunning())
           return;

        int size = data.size();

        QByteArray mid = data;

        if (m_buffer.size() >= size)
        {
            mid = m_buffer.mid(0, size);
            m_buffer.remove(0, size);
        }

        m_audio_lib->inputDataBack(mid);
    });

    connect(m_audio_lib, &AudioStreamingLibCore::error, this, [](QString error){
        qDebug() << error;
    });
    connect(m_audio_lib, &AudioStreamingLibCore::warning, this, [](QString warning){
        qDebug() << warning;
    });

    m_audio_lib->start(info);
    m_audio_lib->listen(quint16(1024), true);
    m_audio_lib->setVolume(m_volume * 100);


    audioDevice = new KSoundDevice(this);
    audioDevice->open(QIODevice::ReadOnly);
    KSoundMix *rootSoundMix = new KSoundMix(true, this);
//    SinSound *waveRadiation2000 = new SinSound(true, this);
//    waveRadiation2000->setFrequency(2000);
//    rootSoundMix->addSound(waveRadiation2000);
//    setRootSound(rootSoundMix);

    audioDevice->setRootSound(rootSoundMix);

    m_audioOutput = new QAudioOutput(deviceInfo, format, this);
    m_audioOutput->setBufferSize(8192);
    m_audioOutput->setNotifyInterval(128);

    m_audioOutput->start(audioDevice);

    m_audioOutput->setVolume(m_volume);
}

//!
//! \brief Audio::toggleSuspendResume
//!
void Audio::toggleSuspendResume()
{
    if (m_audioOutput->state() == QAudio::SuspendedState || m_audioOutput->state() == QAudio::StoppedState) {
        m_audioOutput->resume();
        //m_suspendResumeButton->setText(tr("Suspend recording"));
    } else if (m_audioOutput->state() == QAudio::ActiveState) {
        m_audioOutput->suspend();
        //m_suspendResumeButton->setText(tr("Resume playback"));
    } else if (m_audioOutput->state() == QAudio::IdleState) {
        // no-op
    }
}

//!
//! \brief Audio::device
//! \return
//!
KSoundDevice * Audio::device()
{
    return audioDevice;
}

//!
//! \brief Audio::changeVolume
//! \param value
//!
void Audio::changeVolume(double value)
{
    m_volume = QAudio::convertVolume(value,
                                     QAudio::LogarithmicVolumeScale,
                                     QAudio::LinearVolumeScale);
    m_audioOutput->setVolume(m_volume);
    m_audio_lib->setVolume(value);

//    QProcess pChangeVolume;
//#ifdef WIN64
//    auto out = KMath::instance()->dmap(value, 0, 1, 0, 65535);
////    qDebug() << out;
////    if(!QFile("../../../bin/apps/nircmd.exe").open(QFile::ReadOnly)) {
//        qDebug() << QDir().absolutePath();
////    }
//        pChangeVolume.setProgram("../../../utils/nircmdc.exe");
//    pChangeVolume.setArguments(QStringList() << "setsysvolume" << QString::number(out));
//    connect(&pChangeVolume, &QProcess::errorOccurred, this, [&](QProcess::ProcessError err){
//        Q_UNUSED(err)
//        qDebug() << pChangeVolume.errorString();
//    });

//#else
//    pChangeVolume.setArguments(QStringList() << "");
//#endif
//    pChangeVolume.start();
//    pChangeVolume.waitForFinished(50);
//    qDebug() << "volume changed" << m_audioOutput->volume();
}
