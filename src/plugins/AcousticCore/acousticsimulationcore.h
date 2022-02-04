#ifndef ACOUSTICSIMULATIONCORE_H
#define ACOUSTICSIMULATIONCORE_H

#include <QElapsedTimer>
#include <QJSEngine>
#include <QObject>
#include <QQmlEngine>

#include <wave/wavedata.h>
#include <wave/wavegenerator.h>
#include <base/physical_SI.h>
#include <stationscore.h>

class AcousticSimulationCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(WaveGenerator* generator READ generator NOTIFY generatorChanged)
    Q_PROPERTY(StationsCore* stationsCore READ stationsCore NOTIFY stationCoreChanged)
    explicit AcousticSimulationCore(QObject *parent = nullptr);
public:
    static AcousticSimulationCore *instance();
    WaveGenerator* generator() const;
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

public slots:
    StationsCore *stationsCore() const;
    QList<WaveData> getWavesData();
    PhysicalItem *cast(SceneItem *item);

signals:
    void threadStop();
    void generatorChanged();
    void stationCoreChanged();

private:
    WaveGenerator* m_generator = nullptr;
    StationsCore *m_stationsCore = nullptr;
};

#endif // ACOUSTICSIMULATIONCORE_H
