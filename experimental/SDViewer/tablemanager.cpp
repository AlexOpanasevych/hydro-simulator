#include "tablemanager.h"
TableManager::TableManager(QObject *parent) : QObject(parent)
{
    m_model = m_unitList.model();
    m_model->setParent(this);
}
TableUnit::TableUnit(QObject *parent) : QObject(parent) {}
TableUnit *TableManager::getUnit(QVariantMap map)
{
    int idx = createdTabs.indexOf(map);
    TableUnit *unit = nullptr;
    if (idx == -1)
    {

        unit = new TableUnit(this);
        TableModel * m = new TableModel(this);
        m->loadFromFile(map.value("process", 0).toInt(),
                        map.value("pid", 0).toInt(),
                        map.value("type", 0).toInt(),
                        map.value("recordposition", 0).toInt());
        unit->setModel(m);
        unit->setName(QString::number(++index));
        createdTabs.push_back(map);
        m_unitList.push_back(unit);
        setTablesCount(tablesCount()+1);
        idx = createdTabs.indexOf(map);
    }
    else
    {
        unit = m_unitList.get(idx);
    }
    unit->setMap(map);
    m_filesMap.insert(map.value("process", 0).toString()+'_'+map.value("pid", 0).toString()+'_'+map.value("type", 0).toString()+'_'+ map.value("recordposition", 0).toString(), true);
    emit filesMapChnaged(m_filesMap);

    return unit;
}
void TableManager::deleteUnit(QVariantMap map)
{
    int idx = createdTabs.indexOf(map);
    m_unitList.remove(idx);
    setTablesCount(tablesCount()-1);
    createdTabs.removeAt(idx);
    m_filesMap[map.value("process", 0).toString()+'_'+map.value("pid", 0).toString()+'_'+map.value("type", 0).toString()+'_'+ map.value("recordposition", 0).toString()] = false;
    emit filesMapChnaged(m_filesMap);
}

