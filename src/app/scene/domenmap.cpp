#include "domenmap.h"

DomenMap::DomenMap(QObject *parent) : QObject(parent)
{

}

bool DomenMap::remove(QQuickItem *value)
{
    auto key = m_map.key(value);
    if (value != m_map[key]) return false;

    disconnect(key, nullptr, this, nullptr);
    //disconnect(value, nullptr, this, nullptr);

    return m_map.remove(key);
}

bool DomenMap::remove(QObject *key)
{
    auto value = m_map[key];
    Q_UNUSED(value)
    disconnect(key, nullptr, this, nullptr);
    //disconnect(value, nullptr, this, nullptr);

    return m_map.remove(key);
}

void DomenMap::insert(QObject *key, QQuickItem *value)
{
    connect(key, &QObject::destroyed, this, [this, key](){
        remove(key);
        emit removed();
    });

    //connect(key, &QQuickItem::destroyed, this, [this, value](){
    //    remove(value);
    //});

    m_map.insert(key, value);
}

QQuickItem *DomenMap::get(QObject *key)
{
    auto var = m_map[key];
    return var;
}

void DomenMap::clear()
{
    QMapIterator<QObject *, QQuickItem *> iter(m_map);

    while (iter.hasNext())
    {
        auto item = iter.next();
        remove(item.key());
    }
}

