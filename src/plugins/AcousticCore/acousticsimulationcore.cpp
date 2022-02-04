#include "acousticsimulationcore.h"
#include <QtConcurrent>
#include <cdebug.h>

AcousticSimulationCore::AcousticSimulationCore(QObject *parent) : QObject(parent), m_stationsCore(new StationsCore(this))
{

}

AcousticSimulationCore *AcousticSimulationCore::instance()
{
    static AcousticSimulationCore *m_instance;
    if (m_instance == nullptr) {
        m_instance = new AcousticSimulationCore();
    }
    return m_instance;
}

WaveGenerator *AcousticSimulationCore::generator() const
{
    return m_generator;
}

QObject *AcousticSimulationCore::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) {
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)
    return instance();
}


QList<WaveData> AcousticSimulationCore::getWavesData()
{
    return {};
}

PhysicalItem *AcousticSimulationCore::cast(SceneItem *item)
{
    return qobject_cast<PhysicalItem*>(item);
}

StationsCore *AcousticSimulationCore::stationsCore() const
{
    return m_stationsCore;
}
