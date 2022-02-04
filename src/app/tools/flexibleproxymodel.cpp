#include "flexibleproxymodel.h"

FlexibleProxyModel::FlexibleProxyModel(QObject *parent): QSortFilterProxyModel(parent)
{
}

QAbstractItemModel *FlexibleProxyModel::source() const
{
    return sourceModel();
}

void FlexibleProxyModel::setSource(QAbstractItemModel *source)
{
    setSourceModel(source);
}

void FlexibleProxyModel::setFilter(std::function<bool (QObject *)> lambda)
{
    filter = lambda;
    invalidate();
}

int FlexibleProxyModel::roleKey(const QByteArray &role) const
{
    return roleNames().key(role, -1);
}

bool FlexibleProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    auto model = sourceModel();
    QModelIndex sourceIndex = model->index(source_row, 0, source_parent);

    if (filter != nullptr)
    {
        auto obj = qvariant_cast<QObject *>(model->data(sourceIndex));
        return filter(obj);
    }

    return true;
}

