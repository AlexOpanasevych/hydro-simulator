#ifndef KSYSINFO_H
#define KSYSINFO_H

#include <QObject>
#include <kmacro.h>

class KLIBCORELITE_EXPORT KSysInfo : public QObject {
    Q_OBJECT
    K_PRIVATE_SINGLETON(KSysInfo)
    K_QML_SINGLETON(KSysInfo)

    K_READONLY_PROPERTY(QByteArray, machineUniqueId, machineUniqueId, setMachineUniqueId, machineUniqueIdChanged, QByteArray())
    K_READONLY_PROPERTY(QString, machineHostName, machineHostName, setMachineHostName, machineHostNameChanged, QString())
public:
    explicit KSysInfo(QObject *parent = nullptr);

signals:

};

#endif // KSYSINFO_H
