#ifndef WAVEGENERATOR_H
#define WAVEGENERATOR_H

#include <basemovecontroller.h>
#include <circularbuffer.h>

#include "waveservice.h"
#include <sounds/rawsound.h>

class WaveGenerator
{
public:
    WaveGenerator(AbstractEngine * engine);

    void proceed();
    void registerController(int id);

    void setEngine(AbstractEngine *value);
    RawSound * sound = new RawSound("../AudioModule/sounds/shipSound.sd");
private:
    void generateNoise(int id);
private:
    std::map<int, CircularBuffer<double>> noises;
    static constexpr auto rCoeff = 0.1; // reflect coeff
    static constexpr auto minPressure = 0.01; // if less then kill wave
    static constexpr auto soundSpeed = 1481; // underwater sound velocity
    static constexpr auto waveCount = 100; // underwater sound velocity
    AbstractEngine * engine = nullptr;
};

#endif // WAVEGENERATOR_H
