#include "objectservice.h"

#include <QDir>
#include <QDir>
#include <QDir>
#include <QJsonDocument>
#include <cdebug.h>

PhysicalObjectsService::PhysicalObjectsService(QObject *parent) : QObject(parent)
{
    model = m_dataList.model();
    model->setParent(this);
    proxy()->setSource(model);
    proxy()->setFilter([](QObject *obj) -> bool{
        return obj->property("type").toString() == "Vessel";
    });

    //connect(this, &PhysicalObjectsService::destroyed, this, &PhysicalObjectsService::save);
    //connect(this, &PhysicalObjectsService::createableTypesChanged, this, [this](){
    //    qDebug() << createableTypes();
    //});
}

void PhysicalObjectsService::load()
{
    setReadySave(false);
    clear();
    setCurrentChoosed(nullptr);
    subElements(QUrl(contentPath).path());

    for (auto data : m_dataList)
    {
        auto torpedos = data->torpedos();
        for (const auto &t : torpedos)
        {
            if (!createableTypes().contains(t))
            {
                qDebug(v) << "|-----------------------------------------------------|";
                qDebug(v) << "|  Config error: " << t << "type doesn't exist";
                qDebug(v) << "|  ObjectName error: " << data->objectName();
                qDebug(v) << "|-----------------------------------------------------|";
                torpedos.removeOne(t);
            }
        }
    }

    //qDebug(v) << createableTypes();
}

void PhysicalObjectsService::save()
{
    setCopying(false);
    setReadySave(false);
    clearRemoved();
    if(m_dataList.count()) {
        for (auto data : m_dataList)
        {
            QFile jsonFile(data->path());
            if (!jsonFile.open(QIODevice::WriteOnly)) { qDebug(v) << "Error opening: " << jsonFile.errorString(); return;  }
            QVariant d = data->serialize();
            jsonFile.write(QJsonDocument::fromVariant(d.toMap()).toJson());
            jsonFile.close();
        }
    }
}

void PhysicalObjectsService::clear()
{
    for (auto data : m_dataList)
    {
        m_dataHash.remove(data->objectName());
        m_dataList.remove(data);
        m_createableTypes.removeOne(data->objectName());
        data->deleteLater();
    }
}

void PhysicalObjectsService::cancelClone()
{
    if (copying())
    {    if (currentChoosed() != nullptr)
        {
            currentChoosed()->setRemoved(true);
        }
        else
        {
            m_dataList.last()->setRemoved(true);
        }
    }
    else if (editing())
    {
        m_dataList.last()->setRemoved(true);
    }
}

void PhysicalObjectsService::add(QString type)
{
    setEditing(true);
    auto pi = new PhysicalObjectData(this);
    pi->setObjectName("default_object_name");
    pi->setType(type);

    connect(pi, &PhysicalObjectData::pressed, this, [this, pi](){
        if (currentChoosed() == pi ) {
            setCurrentChoosed(nullptr);
        }
        else
        {
            setCurrentChoosed(pi);
        }
    });

    connect(pi, &PhysicalObjectData::objectNameChanged, this, [this, pi](){
        QString key = m_dataHash.key(pi);
        emit removedType(key);
        m_createableTypes.removeOne(key);
        m_createableTypes.append(pi->objectName());
        m_dataHash.remove(key);
        m_dataHash.insert(pi->objectName(), pi);
        pi->setPath(pi->path().replace(key+".json", pi->objectName() + ".json"));
        emit createableTypesChanged(m_createableTypes);
        //emit updateCreatedObjects();
    });

    connect(pi, &PhysicalObjectData::removedChanged, this, [this, pi](){
        if (pi->removed())
        {
            m_createableTypes.removeOne(pi->objectName());
            emit createableTypesChanged(m_createableTypes);
        }
        else
        {
            if (m_createableTypes.contains(pi->objectName()))
            {
                m_createableTypes.push_back(pi->objectName());
                emit createableTypesChanged(m_createableTypes);
            }
        }
        proxy()->invalidate();
    });

    pi->setPath(QUrl(contentPath + pi->type().toLower() + "/" + pi->objectName() + ".json").path());

    setCurrentChoosed(pi);

    m_dataList.push_back(pi);
    m_dataHash.insert(pi->objectName(), pi);
}

void PhysicalObjectsService::clone()
{
    auto cp = currentChoosed();
    auto pi = new PhysicalObjectData(this);
    QString name = cp->objectName() + "_copy";
    int numb = 0;
    while (true)
    {
        QString nname = name + (numb == 0 ? "" : QString::number(numb));
        if (!createableTypes().contains(nname))
        {
            pi->setPath(pi->path().replace(pi->objectName(), nname));
            pi->setObjectName(nname);
            break;
        }
        numb++;
    }

    pi->deserialize(cp);

    connect(pi, &PhysicalObjectData::pressed, this, [this, pi](){
        if (currentChoosed() == pi ) {
            setCurrentChoosed(nullptr);
        }
        else
        {
            setCurrentChoosed(pi);
        }
    });

    connect(pi, &PhysicalObjectData::objectNameChanged, this, [this, pi](){
        QString key = m_dataHash.key(pi);
        emit removedType(key);
        m_createableTypes.removeOne(key);
        m_createableTypes.append(pi->objectName());
        m_dataHash.remove(key);
        m_dataHash.insert(pi->objectName(), pi);
        pi->setPath(pi->path().replace(key + ".json", pi->objectName() + ".json"));
        emit createableTypesChanged(m_createableTypes);
        //emit updateCreatedObjects();
    });

    connect(pi, &PhysicalObjectData::removedChanged, this, [this, pi](){
        if (pi->removed())
        {
            m_createableTypes.removeOne(pi->objectName());
            emit createableTypesChanged(m_createableTypes);
            emit removedType(pi->objectName());
        }
        else
        {
            if (m_createableTypes.contains(pi->objectName()))
            {
                m_createableTypes.push_back(pi->objectName());
                emit createableTypesChanged(m_createableTypes);
            }
        }
    });

    m_dataHash.insert(pi->objectName(), pi);
    m_dataList.push_back(pi);
    m_createableTypes.push_back(pi->objectName());
    setCurrentChoosed(pi);
    setEditing(true);
    setCopying(true);
    emit createableTypesChanged(m_createableTypes);
}

void PhysicalObjectsService::show(QString type)
{
    proxy()->setFilter([type](QObject *obj) -> bool{
        auto r = obj->property("type").toString() == type && !obj->property("removed").toBool();
        return r;
    });
    proxy()->invalidate();
}

bool PhysicalObjectsService::isExist(QString name)
{
    return createableTypes().contains(name);
}

QVariantMap PhysicalObjectsService::params(QString objectName)
{
    if (m_dataHash.contains(objectName))
    {
        QVariantMap m = m_dataHash[objectName]->serialize();
        if (m.value("imagePath", "").toString() != "")
        {
            auto p = QUrl(QDir::currentPath() + "/" + m.value("imagePath", "").toString());
            //qDebug() << p;
            m["imagePath"] = p;
        }
        return m;
    }
    return {};
}

void PhysicalObjectsService::subElements(QString path)
{
    QDir dir(QUrl(path).path());
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    dir.setSorting(QDir::Size | QDir::Reversed);

    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo fileInfo = list.at(i);

        if (fileInfo.isDir())
        {
            subElements(fileInfo.filePath());
        }
        else
        {
            if (fileInfo.fileName().contains(".json"))
            {
                QFile file(fileInfo.canonicalFilePath());
                if (!file.open(QIODevice::ReadOnly)) qDebug(v) << "Can't open file: " << file.fileName();
                {
                    QByteArray saveData = file.readAll();
                    QVariantMap var = QJsonDocument::fromJson(saveData).toVariant().toMap();
                    //common
                    auto pi = new PhysicalObjectData(this);

                    connect(pi, &PhysicalObjectData::pressed, this, [this, pi](){
                        if (currentChoosed() == pi ) {
                            setCurrentChoosed(nullptr);
                        }
                        else
                        {
                            setCurrentChoosed(pi);
                        }
                    });

                    connect(pi, &PhysicalObjectData::objectNameChanged, this, [this, pi](){
                        QString key = m_dataHash.key(pi);
                        emit removedType(key);
                        m_createableTypes.removeOne(key);
                        m_createableTypes.append(pi->objectName());
                        m_dataHash.remove(key);
                        m_dataHash.insert(pi->objectName(), pi);
                        pi->setPath(pi->path().replace(key+".json", pi->objectName() + ".json"));
                        emit createableTypesChanged(m_createableTypes);
                        //emit updateCreatedObjects();
                    });

                    connect(pi, &PhysicalObjectData::removedChanged, this, [this, pi](){
                        if (pi->removed())
                        {
                            m_createableTypes.removeOne(pi->objectName());
                            emit createableTypesChanged(m_createableTypes);
                        }
                        else
                        {
                            if (m_createableTypes.contains(pi->objectName()))
                            {
                                m_createableTypes.push_back(pi->objectName());
                                emit createableTypesChanged(m_createableTypes);
                            }
                        }
                    });

                    pi->deserialize(var);
                    pi->setPath(fileInfo.canonicalFilePath());

                    m_dataList.push_back(pi);
                    m_dataHash.insert(pi->objectName(), pi);
                    //m_createableTypes.push_back(pi->objectName());
                    //emit createableTypesChanged(m_createableTypes);
                    file.close();
                }
            }
        }
    }
}

void PhysicalObjectsService::clearRemoved()
{
    if(m_dataList.count() > 0) {
        for (auto data : m_dataList)
        {
            if (data->removed())
            {
                m_dataList.remove(data);
                m_dataHash.remove(data->objectName());
                disconnect(data, nullptr, this, nullptr);

                QFile file(data->path());
                file.remove();

                data->deleteLater();
            }
        }
    }
}

PhysicalObjectData::PhysicalObjectData(QObject *parent) : QObject(parent)
{
    connect(this, &PhysicalObjectData::imagePathChanged, this, [this](){
        if (imagePath().contains("file:///")) {
#ifdef WIN64
            m_imagePath.remove("file:///");
#else
            m_imagePath.remove("file://");
#endif
            setImagePath(getPath(m_imagePath));
            //emit imagePathChanged(m_imagePath);
        }
    });
}

QVariantMap PhysicalObjectData::serialize()
{
    QVariantMap m = QVariantMap{
    { "Type",           type()          },
    { "MaxVelocity",    maxVelocity()   },
    { "MaxDepth",       maxDepth()      },
    { "Removed",        removed()       },
    { "Removeable",     removeable()    },
    { "Createable",     createable()    },
    { "ObjectName",     objectName()    },
    { "imagePath" ,     imagePath()     }
};
    if (torpedos().count() > 0) m.insert("Torpedos", torpedos());

    return m;
}

void PhysicalObjectData::deserialize(QVariantMap map)
{
    setType(map.value("Type", "Error").toString());
    setObjectName(map.value("ObjectName", "Error").toString());
    setRemoveable(map.value("Removeable", true).toBool());
    setMaxDepth(map.value("MaxDepth", -1).toDouble());
    setMaxVelocity(map.value("MaxVelocity", -1).toDouble());
    setImagePath(map.value("imagePath", "").toString());

    if (map.value("Torpedos", QVariantList()).toList().count() > 0)
    {
        QStringList strList;
        QVariantList list = map.value("Torpedos", QVariantList()).toList();
        for (auto l : list)
        {
            strList.append(l.toString());
        }
        setTorpedos(strList);
    }
}

void PhysicalObjectData::deserialize(PhysicalObjectData *obj)
{
    QVariantMap m = obj->serialize();
    QString name = this->objectName();
    QString p = obj->path().replace(obj->objectName(), name);
    deserialize(m);
    setObjectName(name);
    setPath(p);
}

QString PhysicalObjectData::getPath(QString path)
{
    auto p = QDir::currentPath();

#ifdef WIN64
    p.remove(0, 4);
    path.remove(0, 4);
#else
    p.remove(0, 1);
    path.remove(0, 1);
#endif
    QStringList build = p.split("/");
    QStringList pro = path.split("/");
    QString result = "";
    QString name = pro.takeLast();

    std::reverse(std::begin(pro), std::end(pro));
    std::reverse(std::begin(build), std::end(build));

    for (int i = (pro.length() >= build.length() ? build.length() - 1 : pro.length() - 1); i >= 0; i--)
    {
        if (pro.last() == build.last()){
            pro.takeLast();
            build.takeLast();
        }
        else
        {
            break;
        }
    }

    std::reverse(std::begin(pro), std::end(pro));
    std::reverse(std::begin(build), std::end(build));

    if (build.length() != 0)
    {
        for (int i = 0; i < build.length(); i++)
            result += "../";
    }
    return result + pro.join("/") + "/" + name;
}
