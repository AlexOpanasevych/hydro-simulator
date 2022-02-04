#ifndef PLATINA_H
#define PLATINA_H

#include <src/station.h>
#include <src/stations/platinaSubsys/platinareceiver.h>
#include <src/stations/platinaSubsys/platinatransmitter.h>

class SERVERINTERFACE_LIBRARY_EXPORT Platina : public Station
{
    Q_OBJECT
    K_QML_TYPE(Platina)
    PlatinaReceiver * plReceiver = nullptr;
    PlatinaTransmitter * plTransmitter = nullptr;
public:
    explicit Platina(QObject * parent = nullptr);
};

#endif // PLATINA_H
