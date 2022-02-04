#include "xnpressureconvertor.h"

#include <math.h>
//#include <src/lib/utility/convlogger.h>
//#include <src/lib/utility/logger.h>

QVector<double> XnPressureConvertor::sweep() const {
    return m_sweep;
}

//void XnPressureConvertor::setNoiseGenerator(NoiseGenerator *value) {
//    noiseGenerator = value;
//}
#include <QtDebug>
void XnPressureConvertor::internalProceed() {
    auto it = pressures.begin();
    while (it != pressures.end()) {
        if(it->isReadyDelete()) {
            it = pressures.erase(it);
        } else {
            if(it->type() == XnPressure::Type::Reverberation) {
                for(int j = 0; j < m_sweep.size(); ++j) {
                    m_sweep[j] = it->pressure() * KMath::instance()->gaus(it->elapsed() / 100);
                }
            } else if(mode() == SpatialProcessing) {
                int velocityChannel = static_cast<int>(KMath::instance()->dmap(it->velocity(), maxVelocity(), minVelocity(), 0, channalCount() - 1));
                int index = it->xn() * channalCount() + velocityChannel;
                if(index < m_sweep.size() && index >= 0) {
                    m_sweep[index] = it->pressure();
                }
            } else if(mode() == FrequencyProcessing) {
                int index = static_cast<int>(KMath::instance()->dmap(it->velocity(), minVelocity(), maxVelocity(), 0, channalCount() - 1));
                if(index >= 0 && index < m_sweep.size() - 1) {
                    m_sweep[index] += it->pressure();
                }
            }
            ++it;
        }
    }
    auto processPressure = [this](int i, double pressure) {
//        if(noiseGenerator)
//            noiseGenerator->setAmplitude(noiseAmplitude());
//        return gainOut(pressure + (noiseGenerator ? noiseGenerator->generate(i) : 0));
    };

//    for(int i = 0; i < m_sweep.size() - 1; ++i) {
//        m_sweep[i] = processPressure(i, m_sweep[i]);
//    }
}

XnPressureConvertor::XnPressureConvertor(QObject *parent) /*: NullAmplifier(parent)*/ {

//    noiseGenerator = new DelayNoiseGenerator;

}

void XnPressureConvertor::addPressure(double angle, double intensity, double radialVelocity, double emissionTime) {
//    CONVLOG();
    auto function = [](double value) {
        return qFuzzyCompare(value, 0) ? 1.0 : qAbs(sin(value) / value);
    };

    QVariantMap signal;
    QVariantList pressuresVariant;
    auto startPoint = xnRotation() - (xnShiftAngle() * (xnCount() - 1)) * 0.5 + vesselRotation();
    for(int i = 0; i < xnCount(); ++i) {
        double currentXnAngle = KMath::instance()->constrainDegrees(startPoint + xnShiftAngle() * i);
        double pressure = function(KMath::instance()->degreesDifference(angle, currentXnAngle) * 90 / 6 * M_PI / 180) * intensity;
        pressures.push_back(XnPressure(i, pressure, radialVelocity, emissionTime));
        pressuresVariant.push_back(pressure);
    }

    signal["radialVelocity"] = radialVelocity;
    signal["pressures"] = pressuresVariant;
    emit pressureAdded(signal);
}

void XnPressureConvertor::proceed() {
    if(mode() == SpatialProcessing) {
        m_sweep = QVector<double>(channalCount() * xnCount());
    } else if(mode() == FrequencyProcessing) {
        m_sweep = QVector<double>(channalCount());
    }
    internalProceed();
}

void XnPressureConvertor::proceed(double distance) {
    if(mode() == SpatialProcessing) {
        m_sweep = QVector<double>(channalCount() * xnCount() + 1);
    } else if(mode() == FrequencyProcessing) {
        m_sweep = QVector<double>(channalCount() + 1);
    }
    internalProceed();
    m_sweep[m_sweep.size() - 1] = distance;
}
