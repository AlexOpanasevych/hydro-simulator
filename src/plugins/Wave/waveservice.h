#ifndef WAVESERVICE_H
#define WAVESERVICE_H

#include <QObject>
#include <circularbuffer.h>
#include <kmacro.h>

struct WaveData {
    WaveData() {}
    WaveData(WaveData * parent) {this->primary = parent;}

//    double angle;
    std::function<void(double)> ffp = nullptr;// Far-Field Pattern
    double p0; // start emitting pressure
    double currentPressure;
    double x0, y0, z0;
    double currX, currY, currZ;
    double t0;
//    double velocity;
    int index; // index in array
    int array; // index global array
    WaveData * primary = nullptr;
};

class WaveService : public QObject
{
    Q_OBJECT
    K_THREADSAFE_SINGLETON(WaveService)
    K_QML_SINGLETON(WaveService)
public:
    explicit WaveService(QObject *parent = nullptr);
    ~WaveService();

    std::map<int, CircularBuffer<WaveData> > & wavebuff();

signals:

private:
    std::map<int ,CircularBuffer<WaveData> > m_waveChain;
};

#endif // WAVESERVICE_H
