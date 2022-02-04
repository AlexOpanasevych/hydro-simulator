#include "platinatransmitter.h"
#include <src/station.h>
#include <streamconnector.h>
PlatinaTransmitter::PlatinaTransmitter(QObject *parent) : Transmitter(parent)
{
//    StreamConnector::instance()->addStreamSender("platinaWave");
    connect(this, &PlatinaTransmitter::searchAngleChanged, this, [this](SearchAngle searchAngle){
        switch(searchAngle) {
        case SearchAngle::Search120: emissionDuration = 200;
        case SearchAngle::Search60: emissionDuration = 400;
        }
    });

    connect(this, &PlatinaTransmitter::etalonModeChanged, this, [this](EtalonMode etalonMode){
        timesScanned = 0;
        currentFrequencies.clear();
        switch(etalonMode) {
        case EtalonMode::Etalon1: {
            switch (m_scale) {
            case Scale::S1: {
                maxTimeToScan = 1;
                currentFrequencies.push_back(frequencies[4]);
                break;
            }
            case Scale::S2: {
                maxTimeToScan = 3;
                currentFrequencies.insert(currentFrequencies.end(), {frequencies[2], frequencies[3], frequencies[4]});
                break;
            }
            case Scale::S3:
                [[fallthrough]];
            case Scale::S4: {
                maxTimeToScan = 5;
                currentFrequencies.insert(currentFrequencies.end(), {frequencies[0], frequencies[1], frequencies[2], frequencies[3], frequencies[4]});
                break;
            }
            }
            break;
        }
        case EtalonMode::Etalon2: {
            switch (m_scale) {
            case Scale::S1: {
                maxTimeToScan = 1;
                currentFrequencies.push_back(frequencies[1]);
                break;
            }
            case Scale::S2:
                [[fallthrough]];
            case Scale::S3:
                [[fallthrough]];
            case Scale::S4: {
                maxTimeToScan = 2;
                currentFrequencies.insert(currentFrequencies.end(), {frequencies[0], frequencies[1]});
                break;
            }
            }
            break;
        }
        case EtalonMode::Etalon3: {

            switch (m_scale) {
            case Scale::S1: {
                maxTimeToScan = 1;
                currentFrequencies.push_back(frequencies[4]);
                break;
            }
            case Scale::S2:
                [[fallthrough]];
            case Scale::S3:
                [[fallthrough]];
            case Scale::S4: {
                maxTimeToScan = 2;
                currentFrequencies.insert(currentFrequencies.end(), {frequencies[3], frequencies[4]});
                break;
            }
            }

            break;

        }
        }
    });
}

void PlatinaTransmitter::proceed(KObservableList<Resource> &stateList, const StateData &data)
{

    station()->sendChangesList({});
}

void PlatinaTransmitter::init()
{

}
