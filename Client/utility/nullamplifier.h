#ifndef NULLAMPLIFIER_H
#define NULLAMPLIFIER_H

#include "amplifier.h"



class NullAmplifier : public Amplifier {
    Q_OBJECT
public:
    NullAmplifier(QObject *parent = nullptr);

    // Amplifier interface
public:
    double gain(double signal);
};

#endif // NULLAMPLIFIER_H
