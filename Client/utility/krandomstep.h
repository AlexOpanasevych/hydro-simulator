#ifndef KRANDOMSTEP_H
#define KRANDOMSTEP_H


#include <math/kmath.h>

#include <algorithm>

class KRandomStep {
    int cnt = 32;
    int i = std::rand() % cnt;
    double noise = KMath::instance()->random();
public:
    KRandomStep();
    double proceed();
    void setCnt(int value);
};

#endif // KRANDOMSTEP_H
