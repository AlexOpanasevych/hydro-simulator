#include "wavegenerator.h"
#include <QFuture>
#include <QtConcurrent>
#include <math/kmath.h>

WaveGenerator::WaveGenerator(AbstractEngine * engine)
{
    setEngine(engine);
//    VariadicCallback c([this](){
//        auto t = this->engine->engineTime();
//        QtConcurrent::run([this, &t](){
//            for(auto && item : this->engine->getPiControllers()) {
//                    for(int i = 0; i < waveCount; i++) {
//                        auto sampleData = this->engine->getPiSamples()->getLastSample<PhysicalItemSampleData>(item.first);
//                        auto acceleration = sampleData->acceleration;
////                        auto direction = sampleData->direction;
//                        auto position = sampleData->position;
//                        auto waveData = new WaveData;
//                        waveData->x0 = sampleData->position[0];
//                        waveData->y0 = sampleData->position[1];
//                        waveData->z0 = sampleData->position[2];
//                        waveData->currX = sampleData->position[0];
//                        waveData->currY = sampleData->position[1];
//                        waveData->currZ = sampleData->position[2];
//                        waveData->array = item.first;
//                        waveData->t0 = t / 1000000. + i / 100.; // rework this
//                        waveData->p0 = sound->volume(48000 / 100 * i);
//                        waveData->currentPressure = waveData->p0;
//    //                    waveData->index = noises[i.first].get_write_pos();
//                        WaveService::instance()->wavebuff()[item.first].push(waveData);
//                    }
//                //        waveData->velocity =
//            }
//        });
//    });
    std::mutex m;
    m.lock();
//    engine->oneSecondCallbacks.push_back(c);
    m.unlock();


    QtConcurrent::run([this](){

        proceed();

    });



}

void WaveGenerator::proceed()
{
    auto currentT = engine->m_engineTime;
    for(auto && waveChain : WaveService::instance()->wavebuff()){
        for(int i = 0; i < waveChain.second.size(); i++) {



        }
    }
}

void WaveGenerator::generateNoise(int id)
{

}

void WaveGenerator::setEngine(AbstractEngine *value)
{
    engine = value;
}

void WaveGenerator::registerController(int id)
{
    noises.insert({id, CircularBuffer<double>()});
}
