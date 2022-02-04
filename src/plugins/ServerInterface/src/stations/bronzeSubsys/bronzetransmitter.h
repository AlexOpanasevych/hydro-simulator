#ifndef BRONZETRANSMITTER_H
#define BRONZETRANSMITTER_H

#include <src/subsystems/transmitter.h>

class BronzeTransmitter : public Transmitter
{
public:
    BronzeTransmitter();
    void proceed(KObservableList<Resource> &stateList, const StateData &data) override;

    // AbstractSubsystem interface
public slots:
    void init() override;
};

#endif // BRONZETRANSMITTER_H
