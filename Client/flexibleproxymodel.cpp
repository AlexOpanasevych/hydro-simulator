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

void FlexibleProxyModel::setFilter(Filter *filter)
{
    setDynamicSortFilter(true);
    if(filter != this->filter()) {
        m_filter = filter;
        emit filterChanged(m_filter);
        invalidate();
    }
}

Filter *FlexibleProxyModel::filter()
{
    return m_filter;
}

int FlexibleProxyModel::roleKey(const QByteArray &role) const
{
    return roleNames().key(role, -1);
}

bool FlexibleProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    auto model = sourceModel();
    QModelIndex sourceIndex = model->index(source_row, 0, source_parent);

    if (m_filter)
    {
        auto obj = qvariant_cast<QObject *>(model->data(sourceIndex));
        return m_filter->filter(obj);
    }

    return true;
}

