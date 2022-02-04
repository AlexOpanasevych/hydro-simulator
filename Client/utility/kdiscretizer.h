#ifndef KDISCRETIZER_H
#define KDISCRETIZER_H

#include <QVector>


class KDiscretizer {
    int count = 10;
    double lastX;
    bool frst = true;
public:
    KDiscretizer(int count = 10);
    inline QVector<double> calc(double x);
    int getCount() const;
    void setCount(int value);
};

QVector<double> KDiscretizer::calc(double x){
    if (frst) {
        frst = false;
        lastX = x;
        return { x };
    }

    QVector<double> result(count);
    auto dx = (x - lastX) / count;
    for (int i = 0; i < count; ++i) {
        result[i] = lastX + i * dx;
    }

    lastX = x;
    return result;
}

#endif // KDISCRETIZER_H
