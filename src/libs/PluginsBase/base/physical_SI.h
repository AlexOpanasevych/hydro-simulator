#ifndef PHISYCALITEM_H
#define PHISYCALITEM_H

#include <base/sceneitem.h>
#include <engineinterface.h>

class PLUGINSBASE_EXPORT PhysicalItem : public SceneItem
{
    Q_OBJECT
    K_AUTO_PROPERTY(double, x, x, setX, xChanged, 0)
    K_AUTO_PROPERTY(double, y, y, setY, yChanged, 0)
    K_AUTO_PROPERTY(double, z, z, setZ, zChanged, 0)
    K_AUTO_PROPERTY(int, id, id, setId, idChanged, 0)
    K_AUTO_PROPERTY(double, velocity, velocity, setVelocity, velocityChanged, 0)
    K_AUTO_PROPERTY(double, dead, dead, setDead, deadChanged, false)
    K_AUTO_PROPERTY(double, width, width, setWidth, widthChanged, 0)
    K_AUTO_PROPERTY(double, height, height, setHeight, heightChanged, 0)
    K_AUTO_PROPERTY(double, rotation, rotation, setRotation, rotationChanged, 0)
    K_CONST_PROPERTY(int, bufferLength, 300)
public:
    PhysicalItem(QObject *parent = nullptr);

public slots:
    void move(QPointF pos);
signals:
    void moved(QPointF point);
    void coordsUpdated(QPointF point);
    void trajectoryChanged();
public:
    template<typename T> void setupSampleDataGetter(std::function<void(T)>);
    virtual void proceed(SampleDataGetter &getter) override;

    virtual QVariantMap serialize() override { return {}; };
    virtual void deserialize(QVariantMap map) override { Q_UNUSED(map) };
    virtual void init(EngineInterface *ei) override;
private:
    static inline const int PhysicalItemQMLRegistration = qmlRegisterType<PhysicalItem>("PluginsBase", 1, 0, "PhysicalItem");
};

#endif // PHISYCALITEM_H
