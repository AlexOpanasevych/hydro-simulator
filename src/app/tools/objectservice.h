#ifndef PHYSICALOBJECTSERVICE_H
#define PHYSICALOBJECTSERVICE_H

#include <QObject>
#include <libs/klibcorelite/kmacro.h>
#include <libs/klibcorelite/utility/kobservablelist.h>
#include <tools/flexibleproxymodel.h>

class PhysicalObjectData : public QObject
{
    Q_OBJECT
    K_AUTO_PROPERTY(QString , type, type, setType, typeChanged, "")
    K_AUTO_PROPERTY(double, maxVelocity, maxVelocity, setMaxVelocity, maxVelocityChanged, 0)
    K_AUTO_PROPERTY(double, maxDepth, maxDepth, setMaxDepth, maxDepthChanged, 0)
    K_AUTO_PROPERTY(bool, removed, removed, setRemoved, removedChanged, false)
    K_READONLY_PROPERTY(bool, removeable, removeable, setRemoveable, removeableChanged, false)
    K_READONLY_PROPERTY(bool, createable, createable, setCreateable, createableChanged, false)
    K_READONLY_PROPERTY(QString, path, path, setPath, pathChanged, "")
    K_READONLY_PROPERTY(QStringList, torpedos, torpedos, setTorpedos, torpedosChanged, QStringList())
    K_AUTO_PROPERTY(QString, imagePath, imagePath, setImagePath, imagePathChanged, "")
    friend class PhysicalObjectsService;
    public:
        explicit PhysicalObjectData(QObject *parent = nullptr);
public slots:
    QVariantMap serialize();
    void deserialize(QVariantMap map);
    void deserialize(PhysicalObjectData* obj);
signals:
    void pressed();
private:
    QString getPath(QString path);
};

class PhysicalObjectsService : public QObject
{
    Q_OBJECT
    K_CONST_PROPERTY(FlexibleProxyModel *, proxy, new FlexibleProxyModel(this))
    K_AUTO_PROPERTY(bool, editing, editing ,setEditing, editingChanged, false)
    K_AUTO_PROPERTY(bool, copying, copying, setCopying, copyingChanged, false)
    K_AUTO_PROPERTY(bool, readySave, readySave, setReadySave, readySaveChanged, false)
    K_AUTO_PROPERTY(QStringList, createableTypes, createableTypes, setCreateableTypes, createableTypesChanged, QStringList())
    K_AUTO_PROPERTY(PhysicalObjectData *, currentChoosed, currentChoosed, setCurrentChoosed, currentChoosedChanged, nullptr)
public:
    explicit PhysicalObjectsService(QObject *parent = nullptr);

public slots:
    void load();
    void save();
    void clear();
    void cancelClone();
    void add(QString type);
    void clone();
    void show(QString type);
    bool isExist(QString name);
    QVariantMap params(QString objectName);

signals:
    //void updateCreatedObjects();
    void removedType(QString type);
private:
    KObservableList<PhysicalObjectData> m_dataList;
    KObservableModel* model = nullptr;
    QHash<QString, PhysicalObjectData*> m_dataHash;
    QString contentPath = "../../../content/";

    void subElements(QString path);
    void clearRemoved();
};

#endif // PHYSICALOBJECTSERVICE_H
