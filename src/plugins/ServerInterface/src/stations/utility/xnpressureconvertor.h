#ifndef XNPRESSURECONVERTOR_H
#define XNPRESSURECONVERTOR_H

#include <QObject>
#include <math/kmath.h>
#include <kmacro.h>
//#include "delaynoisegenerator.h"
//#include "noisegenerator.h"

#include "xnpressure.h"
//#include "nullamplifier.h"


class XnPressureConvertor : public QObject/*public NullAmplifier*/ {
    Q_OBJECT
    K_AUTO_PROPERTY(double, xnRotation, xnRotation, setXnRotation, xnRotationChanged, 0)
    K_AUTO_PROPERTY(double, vesselRotation, vesselRotation, setVesselRotation, vesselRotationChanged, 0)
    K_AUTO_PROPERTY(double, xnShiftAngle, xnShiftAngle, setXnShiftAngle, xnShiftAngleChanged, 0)
    K_AUTO_PROPERTY(double, minVelocity, minVelocity, setMinVelocity, minVelocityChanged, 0)
    K_AUTO_PROPERTY(double, maxVelocity, maxVelocity, setMaxVelocity, maxVelocityChanged, 0)
    K_AUTO_PROPERTY(double, noiseAmplitude, noiseAmplitude, setNoiseAmplitude, noiseAmplitudeChanged, 1)
    K_AUTO_PROPERTY(int, xnCount, xnCount, setXnCount, xnCountChanged, 0)
    K_AUTO_PROPERTY(int, channalCount, channalCount, setChannalCount, channalCountChanged, 0)
    QList<XnPressure> pressures;

//    NoiseGenerator *noiseGenerator = nullptr;
    QVector<double> m_sweep;
public:
    enum Mode { SpatialProcessing, FrequencyProcessing };
    Q_ENUM(Mode)
private:
    K_AUTO_PROPERTY(Mode, mode, mode, setMode, modeChanged, SpatialProcessing)
    void internalProceed();
public:
    explicit XnPressureConvertor(QObject *parent = nullptr);
    void addPressure(double angle, double intensity, double radialVelocity, double emissionTime);
    void proceed();
    void proceed(double distance);
    QVector<double> sweep() const;

//    void setNoiseGenerator(NoiseGenerator *value);

signals:
    void pressureAdded(QVariant value);
public slots:
};

#endif // XNPRESSURECONVERTOR_H
