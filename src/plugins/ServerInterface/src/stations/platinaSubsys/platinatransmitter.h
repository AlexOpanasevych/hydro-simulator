#ifndef PLATINATRANSMITTER_H
#define PLATINATRANSMITTER_H

#include <src/subsystems/transmitter.h>

class PlatinaTransmitter : public Transmitter
{
    Q_OBJECT
    enum class PlatinaMode {O1, O2, C1, C2} ;

    enum class SearchMode {NearSearch, FarSearch, FarNearSearch} ;

    enum class Side {Left, Right};

    enum class SearchAngle {Search120, Search60} ;

    enum class EtalonMode {Etalon1, Etalon2, Etalon3};

    enum class Scale {S1, S2, S3, S4};

    K_READONLY_PROPERTY(SearchAngle, searchAngle, searchAngle, setSearchAngle, searchAngleChanged, SearchAngle::Search120)
    K_READONLY_PROPERTY(EtalonMode, etalonMode, etalonMode, setEtalonMode, etalonModeChanged, EtalonMode::Etalon1)
    K_READONLY_PROPERTY(Scale, scale, scale, setScale, scaleChanged, Scale::S1)
    K_READONLY_PROPERTY(Side, searchSide, searchSide, setSearchSide, searchSideChanged, Side::Left)
    K_READONLY_PROPERTY(SearchMode, searchMode, searchMode, setSearchMode, searchModeChanged, SearchMode::NearSearch)
    K_READONLY_PROPERTY(PlatinaMode, platinaMode, platinaMode, setPlatinaMode, platinaModeChanged, PlatinaMode::O1)

    int emissionDuration = 200; // milliseconds
    int stepDuration = 2500;

    const std::array<double, 7> frequencies = {1, 2, 3, 4, 5, 6, 7};
    std::vector<double> currentFrequencies;

    int timesScanned = 0;

    int maxTimeToScan;
public:
    explicit PlatinaTransmitter(QObject * parent = nullptr);
    void proceed(KObservableList<Resource> &stateList, const StateData &data) override;

    // AbstractSubsystem interface
public slots:
    void init() override;
};

#endif // PLATINATRANSMITTER_H
