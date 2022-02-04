#ifndef STATIONSCORE_H
#define STATIONSCORE_H

#include <QObject>
#include <utility/kobservablelist.h>
#include <station/stationdata.h>
#include <station/stationdata.h>

class StationsCore : public QObject
{
    Q_OBJECT
    Q_PROPERTY(KObservableModel* stationsModel READ stationsModel WRITE setStationsModel NOTIFY stationsModelChanged)
    Q_PROPERTY(StationData* currentItem READ currentItem WRITE setCurrentItem NOTIFY currentItemChanged)
public:
    explicit StationsCore(QObject *parent = nullptr);

public slots:
    StationData *currentItem() const;
    KObservableModel* stationsModel() const;

    void setCurrentItem(StationData *newCurrentItem);

    bool hasStation(const QString &type, PhysicalItem *item);
    void addStation(const QString &stationType, PhysicalItem *item);
    void loadExtensionData(PhysicalItem *item);
    void clearStations();
signals:
    void stationsModelChanged(KObservableModel* stationsModel);
    void currentItemChanged();

private:
    KObservableModel* m_stationsModel;
    KObservableList<StationData> m_stationList;
    QVariantMap extensionData(PhysicalItem *data);
    void removeStationConnection(StationData *station);
    void setStationsModel(KObservableModel* stationsModel);
    void addStationConnection(StationData *station, PhysicalItem *sceneItem);
    StationData *m_currentItem = nullptr;
};

#endif // STATIONSCORE_H
