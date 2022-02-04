#include "wavecontroller.h"

#include <base/global_messages/createpimsg.h>

WaveController::WaveController()
{

}

WaveController::~WaveController()
{
    delete service;
}

void WaveController::proceed(long long, double dt)
{

}

void WaveController::init(BaseMsg *msg, AbstractEngine *e)
{
    setEngine(e);
    service = new WaveService();
    auto createPIMsg = static_cast<CreatePIMsg*>(msg);
    auto startAngle = createPIMsg->angle;
    auto position = createPIMsg->position;
    auto velocity = createPIMsg->velocity;

}

bool WaveController::move(PhysicalItemSampleData * sampleData, double dt)
{

//    auto waveData = service->wavebuff()->peek();


}

void WaveController::setEngine(AbstractEngine *value)
{
    engine = value;
}
