#ifndef FLEXIBLEPROXYMODEL_H
#define FLEXIBLEPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <functional>

class FlexibleProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT
public:
    Q_PROPERTY(QAbstractItemModel *source READ source WRITE setSource)

public:
    FlexibleProxyModel(QObject *parent = nullptr);

    QAbstractItemModel *source() const;
    void setSource(QAbstractItemModel *source);

    void setFilter(std::function<bool(QObject*)>);

signals:
    void countChanged();
    void filterStringChanged();

protected:
    virtual bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;

private:
    int roleKey(const QByteArray &role) const;
    std::function<bool (QObject*)> filter = nullptr;
};


#endif // FLEXIBLEPROXYMODEL_H
