#include "amplifier.h"
Amplifier::Amplifier(QObject *parent) : QObject(parent) {}
double Amplifier::gainOut(double signal) {
    return gain(lastOut(signal));
}
void Amplifier::gainIn(Amplifier *object, double (Amplifier::*function)(double)) {
    lastOut = [object, function](double signal) {
        return (object->*function)(signal);
    };
}
