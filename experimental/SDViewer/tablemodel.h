#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QDir>
#include <QDebug>
#include <kmacro.h>

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
    K_QML_TYPE(TableModel)
public:
    explicit TableModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Q_INVOKABLE QVariant headerData(int nsection,Qt::Orientation orientation,int nRole = Qt::DisplayRole) const override;
public slots:
    void sort(int column, Qt::SortOrder order) override;
    void loadFromFile(int processId , int pid , int type , int recordPosition);
    int sii();
private:
    std::map<int , std::pair<int , std::pair <int , int> >> m_info;
    std::vector<std::vector<double> >  m_table;
    int rowNum = -1;
    int colNum = -1;
    int n_col = 0;
};

#endif // TABLEMODEL_H
