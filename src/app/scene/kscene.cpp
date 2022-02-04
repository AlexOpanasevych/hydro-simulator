#include "componentsprovider.h"
#include "kscene.h"

#include <libs/PluginsBase/base/global_messages/createpimsg.h>
#include <libs/PluginsBase/engine/messages/setshippathmsg.h>
#include <libs/PluginsBase/base/global_messages/removeitemmessage.h>
#include <libs/PluginsBase/base/global_messages/setmodeenginemsg.h>

#include <QQmlComponent>
#include <QVariantList>
#include <QVariant>
#include <QJsonDocument>
#include <libs/PluginsBase/base/physical_SI.h>

#include <libs/PluginsBase/base/sceneitem.h>
#include <libs/PluginsBase/iphysicalitem.h>

#include <libs/PluginsBase/tools/trajectoryparser.h>
#include <libs/PluginsBase/factories/sceneitemsfactory.h>
#include <engine/messages/changecoordsmsg.h>
#include <cdebug.h>
#include <tools/pluginsmanager.h>

KScene::KScene(QObject *parent) : SceneBase(parent)
{
    m_sceneItems = sceneItemsList.model();
    m_sceneItems->setParent(this);
    m_cprovider = new ComponentsProvider(this);
    timer = new QTimer(this);

    connect(timer, &QTimer::timeout, this, [this](){
        this->updateItems();
    });

    //engine connections
    connect(scenePlayer(), &KScenePlayer::startSignal, this, [this](){

        if(scenePlayer()->prevMode() == KScenePlayer::STOP) {

            for(auto item : sceneItemsList) {
                item->init(this->ei);
            }

        }

        SetModeEngineMsg startEngineMsg;
        startEngineMsg.mode = EngineMode::START;
        this->ei->sendToEngine(startEngineMsg).onCompleted<SetModeEngineMsgAns>([this](const SetModeEngineMsgAns & ans){
            qDebug(v) << ":start" << ans.modeChangedSuccess;
            save();
            this->timer->start(50);
        });

    });

    connect(scenePlayer(), &KScenePlayer::stopSignal, this, [this](){
        SetModeEngineMsg startEngineMsg;
        startEngineMsg.mode = EngineMode::RESET;
        this->timer->stop();
        this->ei->sendToEngine(startEngineMsg).onCompleted<SetModeEngineMsgAns>([this](const SetModeEngineMsgAns & ans){
            qDebug(v) << ":stop" << ans.modeChangedSuccess;
            load();

        });

    });

    connect(scenePlayer(), &KScenePlayer::pauseSignal, this, [this](){
        SetModeEngineMsg startEngineMsg;
        startEngineMsg.mode = EngineMode::PAUSE;
        this->ei->sendToEngine(startEngineMsg).onCompleted<SetModeEngineMsgAns>([this](const SetModeEngineMsgAns & ans){

            qDebug(v) << ":pause" << ans.modeChangedSuccess;
            this->timer->stop();
        });

    });

    connect(piService(), &PhysicalObjectsService::removedType, [this](QString type){
        for (auto item : sceneItemsList)
        {
            if (!piService()->createableTypes().contains(item->objectName()))
            {
                if (item->objectName() == type)
                    removeItem(item);
            }
        }
    });

    connect(this, &KScene::engineChanged, m_cprovider, &ComponentsProvider::setEngine);
    proxy()->setSource(sceneItems());
}

void KScene::removeItem(SceneItem *item)
{
    if (ei != nullptr && item && !item->property("dead").toBool() && scenePlayer()->mode() != KScenePlayer::STOP)
    {
        RemoveItemMessage msg;
        msg.ids.push_back(item->property("id").toInt());
        this->ei->sendToEngine(msg).onCompleted<RemoveItemMessageAns>([this, item](const RemoveItemMessageAns & ans){
            if(ans.removed){
                sceneItemsList.remove(item);
                createdControllers.remove(QString::number(item->sceneId()));
                item->metaObject()->method(item->metaObject()->indexOfSlot("setIsDead")).invoke(item);
                item->deleteLater();
            }
        });
    }
    else
    {
        sceneItemsList.remove(item);
        item->deleteLater();
    }
}

void KScene::removeAllItems(QString type)
{
    for (int i = sceneItemsList.count() - 1; i >= 0; i--)
    {
        auto item = sceneItemsList.get(i);
        if (item->type() == type || type == "")
        {
            sceneItemsList.remove(i);
            item->deleteLater();
        }
    }
}

void KScene::setEngineInterface(EngineInterface *ei)
{
    if(ei != nullptr){
        this->ei = ei;
        ei->installStreamMsg<CreatePIMsg>([this](CreatePIMsg msg, int itemId){
            SceneItem *item = nullptr;
            item = addItem(QPointF(msg.position[0], msg.position[1]), msg.typeOfObject);
            item->setProperty("id", itemId);
            item->setProperty("z", msg.position[2]);
            item->setProperty("rotation", msg.angle);
            item->setProperty("velocity", msg.velocity);
            return item;
        });
    }
}

void KScene::updateItems()
{
    for(auto item : sceneItemsList) {
        auto i = qobject_cast<SceneItem*>(item);
        SampleDataGetter g(this->ei);
        i->proceed(g);
    }

    scenePlayer()->updateTime();
}

void KScene::activateFilter(QString type)
{
    proxy()->setFilter([type](QObject *obj) -> bool{
        if (obj) return (obj->property("type").toString() == type);
        return false;});
}

SceneItem *KScene::addItem(QPointF pos, QString objectName)
{
    SceneItem * item = nullptr;
    auto type = piService()->params(objectName).value("Type", "T").toString();

    if (creatableItems().contains(type)){
        QVariantMap m{{"x", pos.x()}, {"y" , pos.y()}};
        item = factoriesHash[type]->create(m);
        item->setObjectName(objectName);
        item->setParent(this);
        item->setSceneId(++m_sceneId);
        createdControllers.insert(QString::number(item->sceneId()), piService()->params(objectName));
        sceneItemsList.push_back(item);
    }
    return item;
}

SceneItem *KScene::addItem(double x, double y, QString type)
{
    return addItem(QPointF(x, y), type);
}

void KScene::addFactory(QList<SceneItemsFactory *> factories)
{
    for (auto f : factories){
        qDebug(v) << f->objectName();
        if (!m_creatableItems.contains(f->objectName())){
            factoriesList.push_back(f);
            m_creatableItems.push_back(f->objectName());
            factoriesHash.insert(f->objectName(), f);
            emit creatableItemsChanged(m_creatableItems);
        }
    }

    piService()->load();
}

QQmlComponent * KScene::getComponent(SceneItem *object, QString type)
{
    return getComponent("scene", object->metaObject()->className(), type);
}

QVariantList KScene::getComponents(QString type)
{
    QVariantList list;
    bool sorted = false;
    int currentPriority = 1;
    while(!sorted){
        sorted = true;
        for (auto iter = allComponents.begin(), end = allComponents.end(); iter != end; iter++){
            for (auto iter2 = iter.value().begin(), end2 = iter.value().end(); iter2 != end2; iter2++){
                int priority = PluginsManager::instance()->priority(iter2.key());
                if (priority >= currentPriority) sorted = false;
                if (priority == currentPriority && iter2.value().contains(type)){
                    QQmlComponent * component = getComponent(iter.key(), iter2.key(), type);
                    if (component != nullptr)
                        list.push_back(QVariant::fromValue<QQmlComponent*>(component));
                    break;
                }
            }
        }
        currentPriority++;
    }

    if (type == "topBar"){
        list.push_front(QVariant::fromValue<QQmlComponent*>(m_cprovider->createComponent(QUrl("qrc:/resources/qml/topBar/AdditionalInfoButton.qml"))));
        list.push_front(QVariant::fromValue<QQmlComponent*>(m_cprovider->createComponent(QUrl("qrc:/resources/qml/topBar/MoveButton.qml"))));
        list.push_front(QVariant::fromValue<QQmlComponent*>(m_cprovider->createComponent(QUrl("qrc:/resources/qml/topBar/CenterButton.qml"))));
        if (adminMode())
            list.push_front(QVariant::fromValue<QQmlComponent*>(m_cprovider->createComponent(QUrl("qrc:/resources/qml/topBar/DevButton.qml"))));
        list.push_front(QVariant::fromValue<QQmlComponent*>(m_cprovider->createComponent(QUrl("qrc:/resources/qml/topBar/DeleteButton.qml"))));

        for (int i = 0; i < list.count() / 2; i++){
            list.insert(i * 2 + 1, QVariant::fromValue<QQmlComponent*>(m_cprovider->createComponent(QUrl("qrc:/resources/qml/topBar/Separator.qml"))));
        }
    }
    else if (type == "menuButton"){
        if (factoriesList.count() != 0){
            list.push_back(QVariant::fromValue<QQmlComponent*>(m_cprovider->createComponent(QUrl("qrc:/resources/qml/delegates/properties/TargetDelegateButton.qml"))));
        }
        list.push_back(QVariant::fromValue<QQmlComponent*>(m_cprovider->createComponent(QUrl("qrc:/resources/qml/delegates/settings/SettingsDelegateFrameButton.qml"))));
    }else if (type == "menuWindow"){
        if (factoriesList.count() != 0){
            list.push_back(QVariant::fromValue<QQmlComponent*>(m_cprovider->createComponent(QUrl("qrc:/resources/qml/delegates/properties/TargetDelegateWindow.qml"))));
        }
        list.push_back(QVariant::fromValue<QQmlComponent*>(m_cprovider->createComponent(QUrl("qrc:/resources/qml/delegates/settings/SettingsDelegateFrameWindow.qml"))));
    }else if (type == "settingsButton"){
        if (adminMode())
            list.push_back(QVariant::fromValue<QQmlComponent*>(m_cprovider->createComponent(QUrl("qrc:/resources/qml/delegates/log/SettingsButtonLog.qml"))));
        list.push_back(QVariant::fromValue<QQmlComponent*>(m_cprovider->createComponent(QUrl("qrc:/resources/qml/delegates/pmanager/PluginsManagerButton.qml"))));
    }else if (type == "settingsWindow"){
        if (adminMode())
            list.push_back(QVariant::fromValue<QQmlComponent*>(m_cprovider->createComponent(QUrl("qrc:/resources/qml/delegates/log/SettingsWindowLog.qml"))));
        list.push_back(QVariant::fromValue<QQmlComponent*>(m_cprovider->createComponent(QUrl("qrc:/resources/qml/delegates/pmanager/PluginsManagerWindow.qml"))));
    }
    return list;
}

QQmlComponent *KScene::getComponent(QString category, QString objectName, QString type)
{
    if (allComponents.contains(category) && allComponents[category].contains(objectName)) {
        if(createdComponents[category].contains(objectName) && createdComponents[category][objectName].contains(type)){
            return createdComponents[category][objectName][type];
        } else {  return createComponent(category, objectName, type); }
    }
    return nullptr;
}

void KScene::addComponents(QMap<QString, QMap<QString, QMap<QString, QString>>> sceneComponents)
{
    for (auto category = sceneComponents.begin(), categoryEnd = sceneComponents.end(); category != categoryEnd; category++){
        PluginPathComponents pluginPathComponents;
        if (allComponents.contains(category.key())) pluginPathComponents = allComponents[category.key()];
        for (auto type = category.value().begin(), typeEnd = category.value().end(); type != typeEnd; type++){
            PathComponents pluginsPathes;
            if (allComponents[category.key()].contains(type.key()))  pluginsPathes = allComponents[category.key()][type.key()];
            for (auto path = type.value().begin(), pathEnd = type.value().end(); path != pathEnd; path++){
                if (!pluginsPathes.contains(path.key())) pluginsPathes.insert(path.key(), path.value());
            }
            pluginPathComponents.insert(type.key(), pluginsPathes);
        }
        allComponents.insert(category.key(), pluginPathComponents);
    }
}

void KScene::load()
{
    this->removeAllItems("");
    QFile jsonFile("SceneItemsDump.json");
    if (!jsonFile.open(QIODevice::ReadOnly)) { qDebug(v) << "Error opening: " << jsonFile.errorString(); return;  }
    QByteArray saveData = jsonFile.readAll();
    QVariantMap var = QJsonDocument::fromJson(saveData).toVariant().toMap();
    jsonFile.close();

    createdControllers.clear();

    QMapIterator iter(var);
    int k = -1;
    while (iter.hasNext()) {
        auto c = iter.next();
        if (c.key() == "controllers") createdControllers.insert(c.key(), c.value().toMap());
        else{
            QString type = c.value().toMap()["type"].toString();
            if (creatableItems().contains(type))
            {
                auto item = factoriesHash[type]->create({});
                item->setParent(this);
                item->deserialize(c.value().toMap());
                sceneItemsList.push_back(item);
            }
        }
        if (k < c.key().toInt()) k = c.key().toInt();
    }
}

void KScene::save()
{
    QVariantMap m;
    for (auto item : sceneItemsList)
    {
        m.insert(QString::number(item->sceneId()), item->serialize());
    }
    QVariantMap contr;
    QHashIterator iter(createdControllers);

    while (iter.hasNext()) {
        auto c = iter.next();
        contr.insert(c.key(), c.value());
    }

    m.insert("controllers", contr);
    QFile jsonFile("SceneItemsDump.json");
    if (!jsonFile.open(QIODevice::WriteOnly)) { qDebug(v) << "Error opening: " << jsonFile.errorString(); return;  }
    jsonFile.write(QJsonDocument::fromVariant(m).toJson());
    jsonFile.close();
}

QQmlComponent * KScene::createComponent(QString category, QString objectName, QString type)
{
    auto component = new QQmlComponent(m_engine, allComponents[category][objectName][type], this);
    createdComponents[category][objectName][type] = component;
    return component;
}

