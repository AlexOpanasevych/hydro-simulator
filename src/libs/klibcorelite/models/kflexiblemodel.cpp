#include "kflexiblemodel.h"
#include <QDebug>
#include <QMapIterator>
#include <QVariant>

KFlexibleModel::KFlexibleModel(QObject *parent) : QAbstractListModel(parent) {}

KFlexibleModel::KFlexibleModel(QVariantList roles, QObject *parent) :QAbstractListModel(parent)
{
    m_names = roles;

    for (int i = 0; i < roles.length(); i++)
    {
        m_roles.insert(i + Qt::UserRole, roles[i].toString().toUtf8());
    }
}

KFlexibleModel& KFlexibleModel::operator =(KFlexibleModel &model)
{
    if (this  == &model)
        return *this;

    beginResetModel();
    this->m_roles = model.m_roles;
    this->m_names = model.m_names;
    this->m_data = model.m_data;
    setSize(model.m_size);
    endResetModel();
    return *this;
}

bool KFlexibleModel::operator ==(KFlexibleModel &model)
{
    if (this->m_names != model.m_names) return false;
    else if (this->m_data != model.m_data) return false;
    return true;
}

bool KFlexibleModel::operator !=(KFlexibleModel &model)
{
    if (this->m_names == model.m_names) return false;
    else if (this->m_data == model.m_data) return false;
    return true;
}

int KFlexibleModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_data.size();
}

QVariant KFlexibleModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() >= m_data.size())
        return QVariant();
    if(role < Qt::UserRole)
        return QVariant();

    return m_data.value(index.row()).value(role);
}

QVariant KFlexibleModel::value(int row, QString role)
{
    if(row < 0 || row >= m_data.size())
        return QVariant("");
    auto a = m_data[row][roleIndex(role)];

    return a;
}

int KFlexibleModel::roleIndex(QString role)
{
    return m_roles.key(role.toUtf8());
}

QHash<int, QByteArray> KFlexibleModel::roleNames() const
{
    return m_roles;
}

QVariantList KFlexibleModel::sortedRoleNames() {
    return m_names;
}

void KFlexibleModel::addRole(QString role)
{
    if (!m_names.contains(role))
    {
        m_names.append(role);
        m_roles.insert(Qt::UserRole + m_roles.size(), role.toUtf8());
    }
}

void KFlexibleModel::addRoles(QStringList rolesList)
{
    for (int i = 0; i < rolesList.length(); i++)
    {
        addRole(rolesList[i]);
    }
}

void KFlexibleModel::addRoles(QVariantList rolesList)
{
    for (int i = 0; i < rolesList.length(); i++)
    {
        addRole(rolesList[i].toString());
    }
}

QVector<QMap<int, QVariant> > KFlexibleModel::allData() const
{
    return m_data;
}

bool KFlexibleModel::isEmpty()
{
    return m_data.isEmpty();
}

void KFlexibleModel::push_back(QVariantList varList)
{
    int pos = m_data.count();
    beginInsertRows(QModelIndex(), pos, pos);
    auto map = generateMap(varList);
    m_data.push_back(map);
    //    qDebug() << "generateMap" << map << m_data;
    setSize(m_data.size());
    endInsertRows();
}

void KFlexibleModel::push_back(QVariantMap varMap)
{
    int pos = m_data.count();
    beginInsertRows(QModelIndex(), pos, pos);
    auto map = generateMap(varMap);
    m_data.push_back(map);
    setSize(m_data.size());
    endInsertRows();
}

void KFlexibleModel::push_front(QVariantList varList)
{
    int pos = 0;
    beginInsertRows(QModelIndex(), pos, pos);
    auto map = generateMap(varList);
    m_data.push_front(map);
    setSize(m_data.size());
    endInsertRows();
}

void KFlexibleModel::push_front(QVariantMap varMap)
{
    int pos = 0;
    beginInsertRows(QModelIndex(), pos, pos);
    auto map = generateMap(varMap);
    m_data.push_front(map);
    setSize(m_data.size());
    endInsertRows();
}

void KFlexibleModel::insert(int pos, QVariantList varList)
{
    if (pos >= m_data.length())
    {
        push_back(varList);
    }
    else if (pos < 0)
    {
        push_front(varList);
    }
    else
    {
        beginInsertRows(QModelIndex(), pos, pos);
        m_data.insert(pos, generateMap(varList));
        endInsertRows();
    }
    setSize(m_data.size());
}

void KFlexibleModel::insert(int pos, QVariantMap varMap)
{
    if (pos >= m_data.length())
    {
        push_back(varMap);
    }
    else if (pos < 0)
    {
        push_front(varMap);
    }
    else
    {
        beginInsertRows(QModelIndex(), pos, pos);
        m_data.insert(pos, generateMap(varMap));
        endInsertRows();
    }
    setSize(m_data.size());
}

bool KFlexibleModel::remove(int pos)
{
    if (pos >= m_data.length() || pos < 0) return false;
    beginRemoveRows(QModelIndex(), pos, pos);

    m_data.remove(pos);
    setSize(m_data.size());
    endRemoveRows();
    return true;
}

bool KFlexibleModel::swap(int pos1, int pos2)
{
    if (pos1 < 0 || pos1 >= m_data.length()) return false;
    if (pos2 < 0 || pos2 >= m_data.length()) return false;
    QVariantMap tmp = lineData(pos1);
    setLineData(lineData(pos2), pos1);
    setLineData(tmp, pos2);
    return true;
}

bool KFlexibleModel::removeAll()
{
    beginResetModel();
    m_data.clear();
    setSize(m_data.size());
    endResetModel();
    return true;
}

void KFlexibleModel::clear()
{
    beginResetModel();
    m_roles.clear();
    m_names.clear();
    m_data.clear();
    setSize(m_data.size());
    endResetModel();
}

QVariant KFlexibleModel::serialize()
{
    auto dataList = m_data.toList();
    QVariantList result;

    for (auto map : qAsConst(dataList)){
        QVariantMap resultMap;
        for (auto r : qAsConst(m_names)){
            QVariant tmp = map.value(roleIndex(r.toString()));
            int pos = -1;

            if (tmp.type()){
                for (auto m : qAsConst(m_roles)){
                    if (m == r){
                        pos = m_roles.key(m);
                        break;
                    }
                }

                if (pos >= 0){
                    resultMap.insert(r.toString(), tmp);
                }
            }else{
                resultMap.insert(r.toString(), "");
            }
        }
        result.push_back(resultMap);
    }

    return result;
}

bool KFlexibleModel::deserialize(QVariant variant, bool clear, bool append)
{
    auto varList = variant.toList();
    if (clear) {
        removeAll();
        beginResetModel();
        m_roles.clear();
        m_names.clear();
        endResetModel();
    }

    //get roles
    if (varList.size() > 0) {
        auto map = varList.first();
        QMap<QString, QVariant> initMap = map.toMap();
        QMapIterator<QString, QVariant> iter(initMap);

        while(iter.hasNext()){
            auto atom = iter.next();
            addRole(atom.key());
        }
    }

    int i = 0;
    if (append){
        i = m_data.size();
    }

    for (const auto &map : qAsConst(varList)){
        QVariantMap m = map.toMap();
        QStringList remList;

        QMapIterator iter(m);
        while(iter.hasNext()){
            auto el = iter.next();
            if (el.value().toString() == ""){
                remList.push_back(el.key());
            }
        }

        while(!remList.empty())
            m.remove(remList.takeFirst());

        setLineData(m, i);
        i++;
    }

    return true;
}

bool KFlexibleModel::contains(QVariantMap varMap)
{
    return indexOf(varMap) != -1;
}

int KFlexibleModel::indexOf(QVariantMap varMap)
{
    for (int i = 0; i < size(); i++){
        QMapIterator iter(varMap);
        bool assert = true;
        while(iter.hasNext()){
            auto atom = iter.next();
            auto val = value(i, atom.key());
            if (val != iter.value()){
                assert = false;
            }
        }
        if (assert) return i;
    }
    return  -1;
}

int KFlexibleModel::indexOf(QVariant value, QString role)
{
    bool singleMode = !(role == "");
    for (const auto &r : singleMode ? QVariantList{role} : m_names){
        for (int i = 0; i < size(); i++){
            auto val = this->value(i, r.toString());
            if (val == value) return i;
        }
    }

    return -1;
}

QVariantList KFlexibleModel::indicesOf(QVariantMap varMap, bool combine)
{
    QVariantList result;
    for (int i = 0; i < size(); i++){
        QMapIterator iter(varMap);
        bool assort = combine ? true : false;
        while(iter.hasNext()){
            auto atom = iter.next();
            auto val = value(i, atom.key());
            if (combine ? (atom.value() != val) : (atom.value() == val)){
                assort = combine ? false : true;
                break;
            }
        }

        if (assort){
            result.push_back(i);
        }
    }
    return result;
}

QVariantList KFlexibleModel::indicesOf(QVariantList varList, bool combine)
{
    QVariantList result;
    for (int i = 0; i < size(); i++){
        bool assort = combine ? true : false;
        auto values = varList;
        for (const QVariant &role : qAsConst(m_names)){
            for (int j = values.size() - 1; j >= 0; j--){
                auto val = value(i, role.toString());
                if (!combine){
                    if (val == values[j]){
                        assort = true;
                        break;
                    }
                }
                if (val == values[j]) values.removeAt(j);
            }
        }

        if (assort){
            if (combine){
                if (values.size() == 0) result.push_back(i);
            }else{
                result.push_back(i);
            }
        }
    }
    return result;
}

QVariantList KFlexibleModel::indicesOf(QVariantList varList, QString role)
{
    QVariantList result;
    if (role == "") return {};
    for (int i = 0; i < size(); i++){
        bool assort = false;
        auto values = varList;
        for (int j = values.size() - 1; j >= 0; j--){
            auto val = value(i, role);

            if (val == values[j]){
                assort = true;
                break;
            }
        }
        if (assort){
            result.push_back(i);
        }
    }
    return result;
}

void KFlexibleModel::generateData()
{
    for (int i = 0; i < 10; i++){
        QMap<int, QVariant> map;
        for (int j = 0; j < m_roles.size(); j++){
            map.insert(j + Qt::UserRole, QString("%1").arg(QString(m_roles[j + Qt::UserRole])));
        }
        m_data.push_back(map);
    }
}

QMap<int, QVariant> KFlexibleModel::generateMap(QVariantList varList)
{
    QMap<int, QVariant> map;
    int i = 0;
    for (const QVariant &r : qAsConst(m_names)){
        if (i >= varList.length()) break;
        map.insert(roleIndex(r.toString()), varList[i]);
        i++;
    }
    return map;
}

QMap<int, QVariant> KFlexibleModel::generateMap(QVariantMap varMap)
{
    QMap<int, QVariant> map;

    for (const auto &r : qAsConst(m_names)){
        QVariant tmp = varMap.value(r.toString());
        int pos = -1;

        if (tmp.type()){
            for (const auto &m : qAsConst(m_roles)){
                if (m == r){
                    pos = m_roles.key(m);
                    break;
                }
            }
            if (pos >= 0) map.insert(pos, tmp);
        }
    }
    return map;
}


bool KFlexibleModel::setLineData(QVariantList varList, int row)
{
    if (m_data.size() - 1 < row)
    {
        push_back(QVariantList());
        setLineData(varList, row);

        return true;
    }

    auto map = generateMap(varList);
    QMapIterator iter(map);

    while(iter.hasNext()){
        auto elem = iter.next();
        m_data[row][elem.key()] = elem.value();
    }

    emit QAbstractListModel::dataChanged(index(row), index(row));
    setSize(m_data.size());
    return true;
}

bool KFlexibleModel::setLineData(QVariantMap varMap, int row)
{
    if (m_data.size() - 1 < row){
        push_back(QVariantList());
        setLineData(varMap, row);

        return true;
    }

    auto map = generateMap(varMap);
    QMapIterator iter(map);

    while(iter.hasNext()){
        auto elem = iter.next();
        m_data[row][elem.key()] = elem.value();
    }

    emit QAbstractListModel::dataChanged(index(row), index(row));
    setSize(m_data.size());
    return true;
}

QVariantMap KFlexibleModel::lineData(int row)
{
    if (row < 0) row = 0;
    else if (row >= m_data.length()) row = m_data.length() - 1;
    QVariantMap result;
    for (const auto &r : qAsConst(m_names)){
        result.insert(r.toString(), value(row, r.toString()));
    }
    return result;
}
