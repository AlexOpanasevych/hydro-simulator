#include "krangedistribution.h"

std::function<double (double)> KRangeDistribution::processor() const {
    return m_processor;
}

void KRangeDistribution::setProcessor(const std::function<double (double)> &processor) {
    m_processor = processor;
}

QVector<double> KRangeDistribution::proceed(const QVector<double> &value) {
    //QVector<double> att = {1, 0.6, 0.5};
    QVector<double> result(value.size());
    for(int i = 0; i < value.size(); ++i) {
        for(int j = 0; j < value.size(); ++j) {
            auto offset = qAbs(i - j);
            if(offset >= 0 && offset < value.size()) {
                result[j] += value[i] * processor()(offset);
                //result[j] += value[i] * att[qAbs(i - j)];
            }
        }
    }
    return result;
}

KRangeDistribution::KRangeDistribution(std::function<double(double)> processor) {
    setProcessor(processor);
}
