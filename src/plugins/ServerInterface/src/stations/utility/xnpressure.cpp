#include "xnpressure.h"
//#include <src/lib/utility/logger.h>
//#include <src/lib/utility/convlogger.h>

int XnPressure::nextId = 0;

double XnPressure::pressure() const {
    return m_pressure;
}

double XnPressure::velocity() const {
    return m_velocity;
}

int XnPressure::xn() const {
    return m_xn;
}

bool operator ==(XnPressure &pressure1, const XnPressure &pressure2) {
    return pressure1.id == pressure2.id;
}

XnPressure::Type XnPressure::type() const {
    return m_type;
}

double XnPressure::emissionTime() const {
    return m_emissionTime;
}

XnPressure::XnPressure(int xn, double pressure, double velocity, double emissionTime, Type type) {

    timer.start();
    m_xn = xn;
    m_pressure = pressure;
    m_velocity = velocity;
    m_type = type;
    m_emissionTime = emissionTime;

    id = nextId++;
}

XnPressure::XnPressure(XnPressure::Type type) : XnPressure(0, 1, 0, type) {/*CONVLOG();*/}

XnPressure::~XnPressure() {
//    CONVLOG();
}

bool XnPressure::isReadyDelete() {
    return timer.elapsed() > emissionTime();
}

qint64 XnPressure::elapsed() {
    return timer.elapsed();
}
