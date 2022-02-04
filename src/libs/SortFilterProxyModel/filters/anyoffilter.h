#ifndef ANYOFFILTER_H
#define ANYOFFILTER_H

#include "filtercontainerfilter.h"
#include <sortfilterproxymodel_global.h>
namespace qqsfpm {

class SORTFILTERPROXYMODEL_EXPORT AnyOfFilter : public FilterContainerFilter {
    Q_OBJECT

public:
    using FilterContainerFilter::FilterContainerFilter;

protected:
    bool filterRow(const QModelIndex& sourceIndex, const QQmlSortFilterProxyModel& proxyModel) const override;
};

}

#endif // ANYOFFILTER_H
