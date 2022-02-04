#ifndef WAVECONTROLLER_H
#define WAVECONTROLLER_H

#include <basemovecontroller.h>
#include <waveservice.h>

class WaveController : public BaseMoveController
{
public:
    WaveController();
    ~WaveController();
// AbstractPIController interface
public:
    void proceed(long long, double dt) override;
    void init(BaseMsg *msg, AbstractEngine *e) override;

// BaseMoveController interface
public:
    bool move(PhysicalItemSampleData *sampleData, double dt) override;
    void setEngine(AbstractEngine *value);

private:
    AbstractEngine * engine = nullptr;
    WaveService * service = nullptr;
};

#endif // WAVECONTROLLER_H
