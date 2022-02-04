#include "audiomodule.h"

AudioModule::AudioModule(QObject * parent)
    : QObject(parent)
{
}

AudioModule::~AudioModule()
{
    m_audioService->deleteLater();
}
