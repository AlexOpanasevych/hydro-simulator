#ifndef RECEIVER_H
#define RECEIVER_H

#include "src/abstractsubsystem.h"
#include "../../serverinterface_global.h"

#include <QTimer>
#include <cdebug.h>

class Receiver : public AbstractSubsystem
{
    QTimer * m_timer = new QTimer(this);
public:
    explicit Receiver(QObject * parent = nullptr);
    virtual void receiverProceed() = 0;
    int order() override;
    QTimer *timer() const;
public slots:
};

#endif // RECEIVER_H
