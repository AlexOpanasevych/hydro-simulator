#ifndef KVECTORDISCRETIZER_H
#define KVECTORDISCRETIZER_H

#include "kdiscretizer.h"

class KVectorDiscretizer {
    int count;
public:
    KVectorDiscretizer(int count = 10);
    QVector<KDiscretizer> scalarDiscretizers;
public:
    inline QVector<QVector<double>> calc(QVector<double> input);
    int getCount() const;
    void setCount(int value);
};

QVector<QVector<double>> KVectorDiscretizer::calc(QVector<double> input) {
    if(input.size() > 0) {
        if(input.size() > scalarDiscretizers.size()) scalarDiscretizers.resize(input.size());

        QVector<QVector<double>> result;
        for(int x = 0; x < input.size(); ++x) {
            scalarDiscretizers[x].setCount(count);
            QVector<double> iVector = scalarDiscretizers[x].calc(input[x]);
            result.resize(iVector.size());
            for(int y = 0; y < iVector.size(); ++y) {
                QVector<double> &line = result[y];
                if(input.size() > line.size()) line.resize(input.size());
                line[x] = iVector[y];
            }
        }
        return result;
    }
    return QVector<QVector<double>>();
}


#endif // KVECTORDISCRETIZER_H
