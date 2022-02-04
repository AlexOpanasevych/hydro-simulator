#ifndef APP_H
#define APP_H

#include <QObject>
#include <kmacro.h>
#include "updateservice.h"
#include <QMetaType>
#include "clientservice.h"
#include "statecommunicator.h"

class App : public QObject
{
    Q_OBJECT
    K_SINGLETON(App)
    K_QML_SINGLETON(App)
    K_AUTO_PROPERTY(QString, resPath, resPath, setResPath, resPathChanged, /*RESOURCES_PATH*/"")
//    K_AUTO_PROPERTY(UpdateService*, updateService, updateService, setUpdateService, updateServiceChanged, new UpdateService(this))
    K_AUTO_PROPERTY(ClientService*, client, client, setClient, clientChanged, new ClientService(this))
    K_AUTO_PROPERTY(QVariantList, menuModel, menuModel, setMenuModel, menuModelChanged, {})
public:
    explicit App(QObject *parent = nullptr);
    enum ViewMode {
        INTERACTIVE = 0x01,
        HELP
    };
    Q_ENUM(ViewMode)
    ~App();

public slots:
    void menuAction(int index)
    {
        funcList[index]();
    }

signals:
private:
    K_AUTO_PROPERTY(ViewMode, viewMode, viewMode, setViewMode, viewModeChanged, INTERACTIVE)
    QList<std::function<void()>> funcList;
};
//Q_DECLARE_METATYPE(std::function<void()>)

#endif // APP_H
