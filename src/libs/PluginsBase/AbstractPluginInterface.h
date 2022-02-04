#ifndef ABSTRACTPLUGININTERFACE_H
#define ABSTRACTPLUGININTERFACE_H

#include <kmacro.h>
#include "pluginsbase_global.h"
#include <QObject>
#include <QVariantList>
#include <abstractengine.h>

typedef QMap<QString, QMap<QString, QMap<QString, QString>>> ComponentsMap;

class PLUGINSBASE_EXPORT AbstractPluginInterface {

    QString m_typeName;
public:
    QString typeName() {return m_typeName;}
    void setTypeName(QString value) {if(value == m_typeName) return; m_typeName = value;}
public:
    AbstractPluginInterface();
    virtual ~AbstractPluginInterface();
    //!
    //! \brief function that does base init manipulations for plugin
    //! \param engine
    //!
    virtual void init(AbstractEngine * engine, QString appPath = "", const QVariantList &depencies = QVariantList()) = 0;
    //!
    //! \brief Returns components that must be created for certain plugin.
    //! \return returns components that must be created for certain plugin
    //!
    virtual ComponentsMap getComponents() = 0;

    template<typename TMsg, typename Callable>
    void registerPluginEngineCalback(AbstractEngine*, Callable f);

};

template<typename TMsg, typename Callable>
        void AbstractPluginInterface::registerPluginEngineCalback(AbstractEngine * engine, Callable f)
{
    engine->getInterface()->addConnection<TMsg>(f);
}

Q_DECLARE_INTERFACE(AbstractPluginInterface, "com.my.AbstractPluginInterface")

#endif // ABSTRACTPLUGININTERFACE_H
