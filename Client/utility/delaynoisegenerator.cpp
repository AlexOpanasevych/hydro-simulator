#include "delaynoisegenerator.h"

#include <math/kmath.h>

double DelayNoiseGenerator::coeficient() const
{
    return m_coeficient;
}

void DelayNoiseGenerator::setCoeficient(double coeficient)
{
    m_coeficient = coeficient;
}

DelayNoiseGenerator::DelayNoiseGenerator(double coeficient) {
    setCoeficient(coeficient);
}
double DelayNoiseGenerator::generate(int i) {
    while ((int)randoms.size() <= i) {
        randoms.push_back(KRandomStep());
    }
    return (randoms[i].proceed() * coeficient() + KMath::instance()->random() * coeficient()) * amplitude();
}
