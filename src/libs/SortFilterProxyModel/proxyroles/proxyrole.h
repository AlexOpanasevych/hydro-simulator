#ifndef PROXYROLE_H
#define PROXYROLE_H

#include <QObject>
#include <QMutex>
#include <sortfilterproxymodel_global.h>

namespace qqsfpm {

class QQmlSortFilterProxyModel;

class SORTFILTERPROXYMODEL_EXPORT ProxyRole : public QObject
{
    Q_OBJECT

public:
    using QObject::QObject;
    virtual ~ProxyRole() = default;

    QVariant roleData(const QModelIndex& sourceIndex, const QQmlSortFilterProxyModel& proxyModel, const QString& name);
    virtual void proxyModelCompleted(const QQmlSortFilterProxyModel& proxyModel);

    virtual QStringList names() = 0;

protected:
    void invalidate();

Q_SIGNALS:
    void invalidated();
    void namesAboutToBeChanged();
    void namesChanged();

private:
    virtual QVariant data(const QModelIndex& sourceIndex, const QQmlSortFilterProxyModel& proxyModel, const QString& name) = 0;

    QMutex m_mutex;
};

}

#endif // PROXYROLE_H
