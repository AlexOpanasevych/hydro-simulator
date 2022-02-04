#ifndef AMPLIFIER_H
#define AMPLIFIER_H

#include <QObject>
#include <functional>

class Amplifier : public QObject {
    std::function<double(double)> lastOut = [](double signal){ return signal; };
public:
    Amplifier(QObject *parent = nullptr);
    double gainOut(double signal);
    void gainIn(Amplifier *object, double(Amplifier::*function)(double));
    virtual double gain(double signal) = 0;

};

#endif // AMPLIFIER_H
