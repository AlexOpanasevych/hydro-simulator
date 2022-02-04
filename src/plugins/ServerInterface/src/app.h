#ifndef APP_H
#define APP_H

#include "updateservice.h"
#include "serverservice.h"
#include <QObject>
#include <kmacro.h>
#include "serverconfigurator.h"
#include "station.h"
#include "../serverinterface_global.h"
#include <cdebug.h>

class SERVERINTERFACE_LIBRARY_EXPORT App : public QObject
{
    Q_OBJECT
    K_AUTO_PROPERTY(ServerConfigurator*, sConfigurator, sConfigurator, setSConfigurator, sConfiguratorChanged, nullptr)
//    K_AUTO_PROPERTY(UpdateService *, updateService, updateService, setUpdateService, updateServiceChanged, new UpdateService(this))
    K_AUTO_PROPERTY(ServerService*, server, server, setService, serverChanged, new ServerService(m_sConfigurator))
    Q_DISABLE_COPY(App)

    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine) { \
        (void)engine; \
        (void)scriptEngine; \
        return instance(); \
    } \
    const int AppQMLRegistration = qmlRegisterSingletonType<App>(KLibInfo::libname, KLibInfo::major, KLibInfo::minor, "App", &App::qmlInstance);
    explicit App(QObject *parent = nullptr);

public:
    static App *instance() { \
        static App *m_instance; \
        if (m_instance == nullptr) { \
            m_instance = new App(); \
        } \
        return m_instance; \
    }
    void initConfigurator(QString appPath);
    ~App() {/*delete m_updateService; emit updateServiceChanged(m_updateService); m_server->deleteLater(); updateServiceChanged(m_updateService); delete m_sConfigurator; sConfiguratorChanged(m_sConfigurator);*/}
signals:

private:
//    Station m_station;
};

#endif // APP_H
