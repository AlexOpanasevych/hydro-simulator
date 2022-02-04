#ifndef POSITIONUNIT_H
#define POSITIONUNIT_H

#include "src/abstractsubsystem.h"
#include "../../serverinterface_global.h"
#include <QObject>
#include <QTimer>
#include <QUdpSocket>


class SERVERINTERFACE_LIBRARY_EXPORT PositionUnit : public AbstractSubsystem
{
    Q_OBJECT
public:
    explicit PositionUnit(QObject * parent = nullptr);

    void proceed(KObservableList<Resource> &stateList, const StateData & data) override;
    int order() override;

    void positionReceived(const QVariantMap &positionInfo);
    QTimer * timer = nullptr;
    QUdpSocket * sock = new QUdpSocket(this);
    ~PositionUnit();

    // AbstractSubsystem interface
public slots:
    void init() override;
};

#endif // POSITIONUNIT_H
