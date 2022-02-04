
#include "krandomstep.h"

void KRandomStep::setCnt(int value) {
    cnt = value;
}

KRandomStep::KRandomStep() {}

double KRandomStep::proceed() {
    if(i-- == 0) {
        i = std::rand() % cnt;
        noise = KMath::instance()->random();
    }
    return noise;
}
