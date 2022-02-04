#include "kdiscretizer.h"

int KDiscretizer::getCount() const {
    return count;
}

void KDiscretizer::setCount(int value) {
    count = value;
}

KDiscretizer::KDiscretizer(int count) {
    this->count = count;
}
