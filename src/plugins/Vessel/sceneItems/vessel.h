#ifndef VESSEL_H
#define VESSEL_H

#include <QObject>
#include <libs/klibcorelite/kmacro.h>
#include <libs/PluginsBase/base/physical_SI.h>
#include <traceData/trace.h>
#include <traceData/trajectory.h>

class Vessel : public PhysicalItem
{
    Q_OBJECT
    K_AUTO_PROPERTY(QString, trajectoryCmds, trajectoryCmds, setTrajectoryCmds, trajectoryCmdsChanged, "")
    K_AUTO_PROPERTY(QVariantList, stages, stages, setStages, stagesChanged, QVariantList())
    K_CONST_PROPERTY(Trace *, trace, new Trace(this))
    K_READONLY_PROPERTY(Trajectory *, trajectory, trajectory, setTrajectory, trajecotryChanged, nullptr)
public:
    Vessel(QObject *parent = nullptr);

    void proceed(SampleDataGetter & getter) override;
    virtual void addExtensionData(const QVariantMap &data) override;
    virtual void removeExtensionData(const QString &datakey) override;
public slots:
    void createTrajectory(QString initialParams = "FORWARD D100 V+30.3 ACC10;\nFORWARD D100 V+10 DZ10 Z50;\nROTATE A+30 R100 Z10;\nFIRE P90 FRA30 SRA30;\nFORWARD D100 V-30 D10;");
    void removeTrajectory();
public:
    virtual QVariantMap serialize() override;
    virtual void deserialize(QVariantMap map) override;


    // SceneItem interface
public:
    void init(EngineInterface *) override;
};

#endif // VESSEL_H
