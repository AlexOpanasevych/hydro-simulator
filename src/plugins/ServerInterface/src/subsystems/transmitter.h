#ifndef TRANSMITTER_H
#define TRANSMITTER_H

#include "src/abstractsubsystem.h"
#include "../../serverinterface_global.h"
#include <QTimer>
//#include <audio.h>

class Transmitter : public AbstractSubsystem
{
    Q_OBJECT


    enum {
        O1 = 0,
        O2,
        C1,
        C2
    };

    enum {
        DP_BP = 0,
        DP,
        BP,
    };
    enum Distance {
        I = 0,
        II,
        III,
        IV
    };

    enum {
        Search120 = 0,
        Search60
    };

    enum {
        NoisePeleng = 0,
        EchoPeleng
    };

public:
    Transmitter(QObject *parent = nullptr);
    void proceed(KObservableList<Resource> &stateList, const StateData & data) override;
    int order() override;
//    AudioModule * module() { return m; }
    ~Transmitter();
private:
    QTimer timer;
    QTimer ep1sweep;
    QTimer platinaSound;
//    AudioModule *m = nullptr;
};

#endif // TRANSMITTER_H
