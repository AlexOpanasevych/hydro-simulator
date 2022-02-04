#ifndef PLATINARECEIVER_H
#define PLATINARECEIVER_H

#include <src/subsystems/receiver.h>
#include "../utility/xnpressureconvertor.h"

#include <qtimer.h>
#include <kmacro.h>

class PlatinaReceiver : public Receiver
{
    Q_OBJECT

    enum class PlatinaMode {O1 = 0, O2, S1, S2, Shp} ;

    enum class SearchMode {FarNearSearch = 0, FarSearch, NearSearch} ;

    enum class Side {Left = 0, Right};

    enum class SearchAngle {Search120 = 0, Search60} ;

    enum class EtalonMode {Etalon1 = 0, Etalon2, Etalon3};

    enum class Scale {Scale1 = 0, Scale2, Scale3, Scale4};

    K_READONLY_PROPERTY(SearchAngle, searchAngle, searchAngle, setSearchAngle, searchAngleChanged, SearchAngle::Search120)
    K_READONLY_PROPERTY(EtalonMode, etalonMode, etalonMode, setEtalonMode, etalonModeChanged, EtalonMode::Etalon1)
    K_READONLY_PROPERTY(Scale, scale, scale, setScale, scaleChanged, Scale::Scale1)
    K_READONLY_PROPERTY(Side, searchSide, searchSide, setSearchSide, searchSideChanged, Side::Left)
    K_READONLY_PROPERTY(SearchMode, searchMode, searchMode, setSearchMode, searchModeChanged, SearchMode::NearSearch)
    K_READONLY_PROPERTY(PlatinaMode, platinaMode, platinaMode, setPlatinaMode, platinaModeChanged, PlatinaMode::O1)
public:
    explicit PlatinaReceiver( QObject * parent = nullptr );
    void proceed(KObservableList<Resource> &stateList, const StateData &data) override;

    void receiverProceed() override;

    inline void search();

    std::vector<double> currentFrequencies;
    XnPressureConvertor xnConvertor;


    // AbstractSubsystem interface
public slots:
    void init() override;


    void onO1o2Changed(const StateData & data);
    void onEpChanged(const StateData &data);
    void onScaleChanged(const StateData & data);
    void onDpbpChanged(const StateData & data);
    void proceedPlatinaModeChange();
    void changeMaxTime();

private:
    void changeMaxTimePrivate(int times);
    StateData m_data;
    const double distanceMultiplier = 4000 / 1481.;
    double maxTime = 2 * distanceMultiplier;

    QTimer * pushTimer = new QTimer;
};
Q_DECLARE_METATYPE(QVector<double>)

#endif // PLATINARECEIVER_H
