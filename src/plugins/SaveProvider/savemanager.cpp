#include "savemanager.h"
#include <base/scenebase.h>
#include <QBuffer>
#include <QJsonDocument>
#include <QPixmap>
#include <QVariant>
#include <cdebug.h>

SaveManager::SaveManager(QObject *parent) : QObject(parent)
{
    m_saves = m_savesList.model();
    m_saves->setParent(this);
    loadConfig();
}

SaveManager::~SaveManager()
{
    saveConfig();
}

void SaveManager::load(SaveData *data)
{
    sceneAccess()->m_scene->removeAllItems("");
    QVariantMap var = data->saveData().toMap();

    sceneAccess()->m_createdControllers->clear();

    QMapIterator iter(var);
    int k = -1;
    while (iter.hasNext()) {
        auto c = iter.next();
        if (c.key() == "controllers") sceneAccess()->m_createdControllers->insert(c.key(), c.value().toMap());
        else{
            QString type = c.value().toMap()["type"].toString();
            if (sceneAccess()->m_scene->creatableItems().contains(type))
            {
                auto item = sceneAccess()->m_factoriesHash->value(type)->create({});
                item->setParent(this);
                item->deserialize(c.value().toMap());
                sceneAccess()->m_sceneItemsList->push_back(item);
            }
        }
        if (k < c.key().toInt()) k = c.key().toInt();
    }
}

void SaveManager::save(SaveData *data)
{
    if (m_savesList.indexOf(data) == -1){
        data->setParent(this);
        m_savesList.push_front(data);
    }
    QVariantMap m;
    for (int i = 0; i < sceneAccess()->m_sceneItemsList->count(); i++)
    {
        auto item = sceneAccess()->m_sceneItemsList->get(i);
        m.insert(QString::number(item->sceneId()), item->serialize());
    }
    QVariantMap contr;
    QHashIterator iter(*sceneAccess()->m_createdControllers);

    while (iter.hasNext()) {
        auto c = iter.next();
        contr.insert(c.key(), c.value());
    }

    m.insert("controllers", contr);
    data->setSaveData(m);
    m_namesList.push_back(data->saveName().toUpper());
    if(data == cacheData()) setCacheData(nullptr);
    setMode(LOOKING);
}

void SaveManager::cancelCreating()
{
    auto d = m_cacheData;
    m_cacheData = nullptr;
    d->deleteLater();
    setMode(LOOKING);
}

void SaveManager::cancelReplacing()
{
    cancelCreating();
}

void SaveManager::cancelEditing()
{
    cancelCreating();
}

void SaveManager::clone(SaveData *data)
{
    setCacheData(new SaveData(this));
    m_cacheData->deserialize(data->serialize());
}

void SaveManager::edit(SaveData *data)
{
    setCurrentData(data);
    clone(data);
    setMode(EDITING);
}

void SaveManager::replace(SaveData *old, SaveData *newOne)
{
    int idx = m_savesList.indexOf(old);
    if (idx == -1){
        m_namesList.push_front(newOne->saveName());
        m_savesList.push_front(newOne);
    }else{
        m_savesList.remove(old);
        m_namesList.removeAll(old->saveName());
        old->deleteLater();
        m_savesList.insert(idx, newOne);
    }
    setMode(LOOKING);
}

void SaveManager::updateCacheData()
{
    if (!cacheData()) return;
    QVariantMap m;
    for (int i = 0; i < sceneAccess()->m_sceneItemsList->count(); i++)
    {
        auto item = sceneAccess()->m_sceneItemsList->get(i);
        m.insert(QString::number(item->sceneId()), item->serialize());
    }
    QVariantMap contr;
    QHashIterator iter(*sceneAccess()->m_createdControllers);

    while (iter.hasNext()) {
        auto c = iter.next();
        contr.insert(c.key(), c.value());
    }

    m.insert("controllers", contr);
    cacheData()->setSaveData(m);
}

void SaveManager::prepareReplacing()
{
    setMode(REPLACING);
}

void SaveManager::removeCurrentData()
{
    m_savesList.remove(currentData());
    currentData()->deleteLater();
    setCurrentData(nullptr);
    setMode(LOOKING);
}

SaveData *SaveManager::createSaveData()
{
    setMode(CREATING);
    setCacheData(new SaveData(this));
    m_cacheData->setDate(QDateTime::currentDateTime().toString("dd.MM.yyyy|hh:mm:ss"));
    return m_cacheData;
}

bool SaveManager::checkName(QString name)
{
    if (mode() == EDITING){
        for (auto s : m_savesList){
            if (s->saveName() == name){
                if (s->date() == cacheData()->date()) return false;
                return true;
            }
        }
    }
    return namesList().contains(name.toUpper());
}

void SaveManager::saveConfig()
{
    std::sort(m_savesList.begin(), m_savesList.end(), [](SaveData *obj1, SaveData *obj2){
        return obj1->idx() <= obj2->idx();
    });

    QVariantList list;
    for (auto d : m_savesList){
        list.append(d->serialize());
    }
    QFile jsonFile("saves.json");
    if (!jsonFile.open(QIODevice::WriteOnly)) { qDebug(v) << "Error opening: " << jsonFile.errorString(); return;  }
    jsonFile.write(QJsonDocument::fromVariant(list).toJson());
    jsonFile.close();
}

void SaveManager::loadConfig()
{
    QFile jsonFile("saves.json");
    if (!jsonFile.open(QIODevice::ReadOnly)) { qDebug(v) << "Error opening: " << jsonFile.errorString(); return;  }
    QByteArray saveData = jsonFile.readAll();
    jsonFile.close();
    QVariantList varList = QJsonDocument::fromJson(saveData).toVariant().toList();
    for (const auto &var : varList){
        auto sd = new SaveData(this);
        sd->deserialize(var);
        m_namesList.push_back(sd->saveName().toUpper());
        m_savesList.push_back(sd);
    }
}
