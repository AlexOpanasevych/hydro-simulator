#ifndef KRANGEDISTRIBUTION_H
#define KRANGEDISTRIBUTION_H

#include <QVector>
#include <functional>

class KRangeDistribution {
    std::function<double(double)> m_processor;
public:
    KRangeDistribution(std::function<double(double)> processor = [](double value){ return value; });
    std::function<double (double)> processor() const;
    void setProcessor(const std::function<double (double)> &processor);
    QVector<double> proceed(const QVector<double> &value);
};

#endif // KRANGEDISTRIBUTION_H
