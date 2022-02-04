#include "kvectordiscretizer.h"

int KVectorDiscretizer::getCount() const {
    return count;
}

void KVectorDiscretizer::setCount(int value) {
    count = value;
}

KVectorDiscretizer::KVectorDiscretizer(int count) {
    this->count = count;
}
