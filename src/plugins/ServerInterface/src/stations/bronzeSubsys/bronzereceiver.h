#ifndef BRONZERECEIVER_H
#define BRONZERECEIVER_H

#include <src/subsystems/receiver.h>

class BronzeReceiver : public Receiver
{
public:
    BronzeReceiver();
    void proceed(KObservableList<Resource> &stateList, const StateData &data) override;
    void receiverProceed() override;

    // AbstractSubsystem interface
public slots:
    void init() override;
};

#endif // BRONZERECEIVER_H
