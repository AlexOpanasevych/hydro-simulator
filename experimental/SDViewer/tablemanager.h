#ifndef TABLEMANAGER_H
#define TABLEMANAGER_H

#include <QObject>
#include <kmacro.h>
#include <utility/kobservablelist.h>
#include "tablemodel.h"
class TableUnit : public QObject
{
    Q_OBJECT
    K_QML_TYPE(TableUnit)
    K_AUTO_PROPERTY(TableModel *, model, model, setModel, modelChanged, nullptr)
    K_AUTO_PROPERTY(QVariantMap , map , map , setMap , mapChanged , QVariantMap())
    K_READONLY_PROPERTY(QString, name, name, setName, nameChanged, "")
    //Q_INVOKABLE QVariant headerData(int nsection,Qt::Orientation orientation,int nRole = Qt::DisplayRole);
    friend class TableManager;
public:
    explicit TableUnit(QObject *parent = nullptr);
};

class TableManager : public QObject
{
    Q_OBJECT
    K_SINGLETON(TableManager)
    K_QML_SINGLETON(TableManager)
    K_READONLY_PROPERTY(KObservableModel *, model, model, setModel, modelChnaged, nullptr)
    K_READONLY_PROPERTY(int, tablesCount, tablesCount, setTablesCount, tablesCountChanged, 0)
    K_READONLY_PROPERTY(QVariantMap, filesMap, filesMap, setFilesMap, filesMapChnaged, QVariantMap())
    public:
    explicit TableManager(QObject *parent = nullptr);

public slots:
    TableUnit * getUnit(QVariantMap map);
    void  deleteUnit(QVariantMap map);
private:
    KObservableList<TableUnit> m_unitList;
    QList<QVariantMap> createdTabs;
    int index = 0;

signals:
};

#endif // TABLEMANAGER_H
