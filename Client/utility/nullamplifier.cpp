#include "nullamplifier.h"

NullAmplifier::NullAmplifier(QObject *parent) : Amplifier(parent) {}

double NullAmplifier::gain(double signal) {
    return signal;
}
