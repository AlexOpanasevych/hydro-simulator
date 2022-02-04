#ifndef CONTROLLERFACTORY_H
#define CONTROLLERFACTORY_H

#include <QObject>
#include <libs/klibcorelite/kmacro.h>
#include <libs/core/basemovecontroller.h>
#include <pluginsbase_global.h>

class PLUGINSBASE_EXPORT ControllerFactory : public QObject
{
    Q_OBJECT
    K_AUTO_PROPERTY(QString, objectName, objectName, setObjectName, objectNameChanged, "")
public:
    ControllerFactory(std::function<AbstractPIController*()> cr): m_creator(cr) {}
    AbstractPIController *create(){ return m_creator();  }
private:
    std::function<AbstractPIController*()> m_creator = nullptr;
};

#endif // CONTROLLERFACTORY_H
