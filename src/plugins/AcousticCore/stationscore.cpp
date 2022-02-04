#include "stationscore.h"

StationsCore::StationsCore(QObject *parent) : QObject(parent)
{
    setStationsModel(m_stationList.model());
    m_stationsModel->setParent(this);
}

KObservableModel *StationsCore::stationsModel() const
{
    return m_stationsModel;
}

bool StationsCore::hasStation(const QString &type, PhysicalItem *item)
{
    if (item != nullptr){
        if (item->extensionData().contains("station")){
            QVariantList list = item->extensionData()["station"].toList();
            for (const auto &l : list){
                if (l.toMap()["type"].toString().toUpper() == type.toUpper()) return true;
            }
        }
    }
    return false;
}

void StationsCore::addStation(const QString &stationType, PhysicalItem *item)
{
    if (item != nullptr){
        if (!hasStation(stationType, item)){
            auto st = new StationData(this);
            st->setX(item->x());
            st->setY(-item->y());
            st->setSceneItem(item);
            st->setMode(stationType.toUpper() == "PLATINA" ? StationData::PLATINA : StationData::BRONZE);
            addStationConnection(st, item);
            m_stationList.push_back(st);
            item->addExtensionData(extensionData(item));
        }
    }
}

void StationsCore::loadExtensionData(PhysicalItem *item)
{
    auto m = item->extensionData();
    if (!m.isEmpty()){
        auto ed = m.value("station", QVariantMap()).toList();
        if (!ed.isEmpty()){
            for (const auto &atom : ed){
                auto dataMap = atom.toMap();
                if (!dataMap.isEmpty()){
                    auto st = new StationData(this);
                    st->setX(item->x());
                    st->setY(-item->y());
                    st->setSceneItem(item);
                    st->setRotation(item->rotation());
                    st->setMode(dataMap.value("type", "BRONZE").toString().toUpper() == "PLATINA" ? StationData::PLATINA : StationData::BRONZE);
                    addStationConnection(st, item);
                    m_stationList.push_back(st);
                }
            }
        }
    }
}

void StationsCore::clearStations()
{
    for (int i = m_stationList.count() - 1; i >= 0; i--){
        auto st = m_stationList.takeAt(i);
        st->sceneItem()->addExtensionData(QVariantMap{{"station" , QVariantList()}});
        disconnect(st->sceneItem(), nullptr, this, nullptr);
        st->deleteLater();
    }
    m_stationList.clear();
}

QVariantMap StationsCore::extensionData(PhysicalItem *data)
{
    QVariantList result;
    for (auto st : m_stationList){
        if (st->sceneItem() == data){
            result.append(QVariantMap{{"type", st->mode() == StationData::PLATINA ? "PLATINA" : "BRONZE"}});
        }
    }
    return QVariantMap{ {"station", result} };
}

void StationsCore::removeStationConnection(StationData *station)
{
    Q_UNUSED(station)
}

void StationsCore::setStationsModel(KObservableModel *stationsModel)
{
    if (m_stationsModel == stationsModel)
        return;

    m_stationsModel = stationsModel;
    emit stationsModelChanged(m_stationsModel);
}

void StationsCore::addStationConnection(StationData *station, PhysicalItem *sceneItem)
{
    if (station != nullptr && sceneItem != nullptr){
        connect(sceneItem, &PhysicalItem::destroyed, this, [this, station, sceneItem](){
            station->setSceneItem(nullptr);
            m_stationList.remove(station);
            disconnect(sceneItem, nullptr, station, nullptr);
            station->deleteLater();
        });

        connect(sceneItem, &PhysicalItem::xChanged, this, [station](double value){
            station->setX(value);
        });
        connect(sceneItem, &PhysicalItem::yChanged, this, [station](double value){
            station->setY(-value);
        });
        connect(sceneItem, &PhysicalItem::rotationChanged, station, &StationData::setRotation);
    }
}

StationData *StationsCore::currentItem() const
{
    return m_currentItem;
}

void StationsCore::setCurrentItem(StationData *newCurrentItem)
{
    if (m_currentItem == newCurrentItem)
        return;
    m_currentItem = newCurrentItem;
    emit currentItemChanged();
}
