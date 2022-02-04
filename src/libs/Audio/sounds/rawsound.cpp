#include "rawsound.h"
#include <QDir>
#include <QFile>
#include <QLoggingCategory>
#include <QTextStream>
#include <QtMath>
#include <qdebug.h>

RawSound::RawSound(QString nameFileRaw, QObject *parent, const bool m_play) : Sound(m_play, parent)
{
    m_countOb++;
    if(!nameFileRaw.isEmpty()) {
        if(!raws.contains(nameFileRaw))
        {
            setNameRaw(nameFileRaw);
            if(!open(nameFileRaw)) return;
        }
        else
        {
            setNameRaw(nameFileRaw);
            return;
        }
    }
}

RawSound::~RawSound()
{
    m_countOb--;
}

//!
//! \brief Function for opening raw audio SD file
//! \param nameFileRaw
//! \return Returns true if file is opened and false if not
//!
bool RawSound::open(QString nameFileRaw){
    if(raws.contains(nameFileRaw)) return true;
//    if(nameFileRaw.endsWith(".mp3")){

//        QAudioFormat format;
//        format.setSampleRate(48000);
//        format.setChannelCount(1);
//        format.setSampleSize(16);
//        format.setCodec("audio/pcm");
//        format.setByteOrder(QAudioFormat::LittleEndian);
//        format.setSampleType(QAudioFormat::SignedInt);


//        decoder.setAudioFormat(format);
//        decoder.setSourceFilename(nameFileRaw);
//        decoder.start();


//    }
//    else {
        QFile dataFile(/*QDir().absoluteFilePath("../resourses/sound/" + nameFileRaw +".sd")*/nameFileRaw);
        if(!dataFile.open(QIODevice::ReadOnly))
        {
            qDebug(QLoggingCategory("global")) << "sound not found" << nameFileRaw;
            throw "Error: File " + /*QDir().absoluteFilePath("../resourses/sound/" + nameFileRaw +".sd")*/nameFileRaw +" not open";
        }

        int         sizebuff = 8;
        char        buff[sizebuff];
        QTextStream in(&dataFile);
        raws.insert(nameFileRaw, QVector<qint16>());

        while (!in.atEnd())
        {
           dataFile.readLine(buff, sizebuff);
           raws[nameFileRaw].push_back(static_cast<qint16>(atoi(buff)));
        }

//    }


    return true;
}

int RawSound::size()
{
    return raws[m_nameRaw].size();
}

//!
//! \brief Overloaded function that generate next sound block for KSoundDevice
//! \return Returns next 8 bytes of sound data
//!
double RawSound::signal(qint64 index)
{

    if(index == -1) {

        double xa       = floor(position);
        double indexTmp = floor(abs(checkPosition()));

        double ya       = raws[m_nameRaw][static_cast<int>(indexTmp)];
        double x        = position;
        indexTmp        = ceil(checkPosition());
        double yb       = raws[m_nameRaw][static_cast<int>(indexTmp)];

        double res = (ya + (yb - ya) * (x - xa))/10000;
        position += m_interpolationFactor;

        return res;

        //stop playing after dedicated time
         if ((position >= m_interval * sampleRate() / 1000) && (m_interval != 0))
        {
            setPlay(false);
            position = 0;
            return res;
        }
    }
    else {

        double ya       = raws[m_nameRaw][index];
        double res = ya/10000;
        return res;
    }

}

//!
//! \brief Get interval for stopping for this sound
//! \return
//!
qint32 RawSound::interval() const
{
    return m_interval;
}

//!
//! \brief Set desired interval in milliseconds for sound, after this interval sound will be stopped
//! \param interval
//!
void RawSound::setInterval(const qint32 &interval)
{
    m_interval = interval;
}

int RawSound::countOb()
{
    return m_countOb;
}

//!
//! \brief Checks position in sound array.
//! If position is lower than array size, we didn't reach end of sound.
//! Else we update position by substracting array size and add interpolation factor, which is 1.0 by default
//! \return
//!
double RawSound::checkPosition()
{
    if(position < raws[m_nameRaw].size() - 1)
    {
        return position;
    }
    else
    {
        position = position - raws[m_nameRaw].size() + m_interpolationFactor;
        return 0;
    }
}

QString RawSound::nameRaw() const
{
    return m_nameRaw;
}

//!
//! \brief Set name of raw audio file
//! \param nameRaw
//!
void RawSound::setNameRaw(const QString &nameRaw)
{
    m_nameRaw = nameRaw;
}

double RawSound::interpolationFactor() const
{
    return m_interpolationFactor;
}


//!
//! \brief Set interpolation factor for sound, which is 1.0 by default
//! \param interpolationFactor
//!
void RawSound::setInterpolationFactor(double interpolationFactor)
{
    m_interpolationFactor = interpolationFactor;
}

//!
//! \brief Helper function for using from plugin
//! \param params
//!
void RawSound::init(QVariantMap params)
{
    Sound::init(params);
    assert(params["name"].isValid());
    setNameRaw(params["name"].toString());
    open(nameRaw());
}
