#ifndef XNPRESSURE_H
#define XNPRESSURE_H

#include <QElapsedTimer>

class XnPressure {
public:
    enum Type { Default, Reverberation };
private:
    static int nextId;
    QElapsedTimer timer;
    double m_pressure = 0.0;
    double m_velocity = 0.0;
    double m_emissionTime = 0.0;
    int m_xn = -1;
    int id = -1;
    Type m_type = Default;
public:
    XnPressure(int xn, double pressure, double velocity, double emissionTime, Type type = Default);
    XnPressure(Type type = Default);
    ~XnPressure();
    qint64 elapsed();
    bool isReadyDelete();
    double pressure() const;
    double velocity() const;
    int xn() const;
    friend bool operator ==(XnPressure& pressure1, const XnPressure& pressure2);
    Type type() const;
    double emissionTime() const;
};

#endif // XNPRESSURE_H
