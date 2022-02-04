#ifndef TORPEDO_H
#define TORPEDO_H

#include <QObject>
#include <libs/klibcorelite/kmacro.h>
#include <traceData/trace.h>
#include <libs/PluginsBase/base/physical_SI.h>

class Torpedo : public PhysicalItem
{
    Q_OBJECT
    K_CONST_PROPERTY(Trace *, trace, new Trace(this))
    K_AUTO_PROPERTY(QVariantList, stages, stages, setStages, stagesChanged, {})
public:
    Torpedo(QObject *parent = nullptr);
    void proceedMove(SampleDataGetter &getter) override;

    virtual QVariantMap serialize() override;
    virtual void deserialize(QVariantMap map) override;
private:
};


#endif // TORPEDO_H
