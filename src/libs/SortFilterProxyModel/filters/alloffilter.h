#ifndef ALLOFFILTER_H
#define ALLOFFILTER_H

#include "filtercontainerfilter.h"
#include <sortfilterproxymodel_global.h>
namespace qqsfpm {

class SORTFILTERPROXYMODEL_EXPORT AllOfFilter : public FilterContainerFilter {
    Q_OBJECT

public:
    using FilterContainerFilter::FilterContainerFilter;

protected:
    bool filterRow(const QModelIndex& sourceIndex, const QQmlSortFilterProxyModel& proxyModel) const override;
};

}

#endif // ALLOFFILTER_H
