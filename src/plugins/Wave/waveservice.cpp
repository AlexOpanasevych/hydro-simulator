#include "waveservice.h"

WaveService::WaveService(QObject *parent) : QObject(parent)
{

}

WaveService::~WaveService()
{
//    delete m_waveChain;
}

std::map<int, CircularBuffer<WaveData>> & WaveService::wavebuff()
{
    return m_waveChain;
}
