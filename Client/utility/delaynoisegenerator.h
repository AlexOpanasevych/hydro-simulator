#ifndef DELAYNOISEGENERATOR_H
#define DELAYNOISEGENERATOR_H

#include "noisegenerator.h"

#include "krandomstep.h"
#include <vector>

class DelayNoiseGenerator : public NoiseGenerator {
    std::vector<KRandomStep> randoms;
    double m_coeficient = 0.05;
public:
    DelayNoiseGenerator(double coeficient = 0.05);

    // NoiseGenerator interface
public:
    double generate(int i);
    double coeficient() const;
    void setCoeficient(double coeficient);
};

#endif // DELAYNOISEGENERATOR_H
