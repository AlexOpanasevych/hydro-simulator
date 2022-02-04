#ifndef FLEXIBLEPROXYMODEL_H
#define FLEXIBLEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <functional>
#include <kmacro.h>
#include "filter.h"

class FlexibleProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
    K_QML_TYPE(FlexibleProxyModel)
    Q_PROPERTY(Filter* filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(QAbstractItemModel *source READ source WRITE setSource)

//
public:
    FlexibleProxyModel(QObject *parent = nullptr);

    QAbstractItemModel *source() const;
    void setSource(QAbstractItemModel *source);

    Q_INVOKABLE void setFilter(Filter * filter);

    Q_INVOKABLE Filter * filter();

signals:
    void countChanged();
    void filterStringChanged();
    void filterChanged(Filter *);

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    int roleKey(const QByteArray &role) const;
    Filter * m_filter = nullptr;
};


#endif // FLEXIBLEPROXYMODEL_H
