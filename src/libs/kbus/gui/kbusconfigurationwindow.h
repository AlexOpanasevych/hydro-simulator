#ifndef KBUSCONFIGURATIONWINDOW_H
#define KBUSCONFIGURATIONWINDOW_H

#include <QAction>
#include "busparams.h"
#include <QFrame>
#include <QWidget>
#include "KBus_global.h"


class KBUS_EXPORT KBusConfigurationWindow: public QFrame
{
    Q_OBJECT
public:
    KBusConfigurationWindow(QString name = "", QString kbusType = "KSerialBusThreaded", QWidget * parent = nullptr);
    ~KBusConfigurationWindow(){};
    void setDefaultBusParams(BusParams params);

signals:
    void connectButtonClicked(QVariantMap data);
    void disconnectButtonClicked();

    void busPortChanged(int port);
    void busSpeedChanged(int speed);

private:
    QString name = "";
    BusParams busParams;

    struct RequestData {
        QString name;
        QString type;
        QWidget * widget;
    };

    QList<RequestData> requestData;
};

#endif // KBUSCONFIGURATIONWINDOW_H
