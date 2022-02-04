#ifndef KINTEGRATOR_H
#define KINTEGRATOR_H

#include <QVector>


class KIntegrator {
    double alpha = 0.9;
    double lastY;
    bool frst = true;
    double specValue;
public:
    KIntegrator(double alpha = 0.9);
    inline double calc(double);
};

double KIntegrator::calc(double x) {
    if (frst) {
        frst = false;
        return (lastY = x);
    }
    if(x > lastY) {
        return (lastY = alpha*lastY + (1-alpha)*x);
    }
    else if(x < lastY){
        return (lastY = alpha*lastY - (1-alpha)*x);
    }
    else return lastY;
}



#endif // KINTEGRATOR_H
