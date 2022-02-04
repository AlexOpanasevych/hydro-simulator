#include "noisegenerator.h"

double NoiseGenerator::amplitude() const
{
    return m_amplitude;
}

void NoiseGenerator::setAmplitude(double amplitude)
{
    m_amplitude = amplitude;
}

NoiseGenerator::NoiseGenerator() {}

NoiseGenerator::~NoiseGenerator() {}
