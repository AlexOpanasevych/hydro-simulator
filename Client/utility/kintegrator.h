#ifndef KINTEGRATOR_H
#define KINTEGRATOR_H

#include <QVector>


class KIntegrator {
    double alpha = 0.9;
    double lastY;
    bool frst = true;
public:
    KIntegrator(double alpha = 0.9);
    inline double calc(double);
};

double KIntegrator::calc(double x) {
    if (frst) {
        frst = false;
        return (lastY = x);
    }

    return (lastY = alpha*lastY + (1-alpha)*x);
}



#endif // KINTEGRATOR_H
