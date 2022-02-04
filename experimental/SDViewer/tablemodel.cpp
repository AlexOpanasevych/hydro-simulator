#include "tablemodel.h"

TableModel::TableModel(QObject *parent)
    : QAbstractTableModel(parent){
    m_info.insert({1 , { 76, {1 , 9}}});
    m_info.insert({2 , { 4, {1 , 0}}});
}
int TableModel::rowCount(const QModelIndex &parent) const
{
    return rowNum;
}
int TableModel::columnCount(const QModelIndex &parent) const
{
    return colNum;
}
QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if(role == Qt::DisplayRole and index.isValid()){
        return m_table[index.row()][index.column()];
    }
    return QVariant();
}

Q_INVOKABLE QVariant TableModel::headerData(int nsection, Qt::Orientation orientation, int nRole) const
{
    if (orientation ==0x2)
    {
        return nsection+1;
    }
    else
    {
        switch (nsection) {
        case 0 : return QString("uSecond");
        case 1 : return QString("position[0]");
        case 2 : return QString("position[1]");
        case 3 : return QString("position[2]");
        default: return QString("others");
        }

    }
}
void TableModel::loadFromFile(int processId, int pid, int type, int recordPosition)
{
    int bytesInOneRow = m_info[type].first;
    int numOfInetegers = m_info[type].second.first;
    int numOfDoubles = m_info[type].second.second;
    n_col = 0;
    QString path;
    path =QDir::currentPath()+'/'+"data"+'/'+QString::fromStdString(std::to_string(processId))+'/'+QString::fromStdString(std::to_string(pid))+'_'+QString::fromStdString(std::to_string(type))+'_'+QString::fromStdString(std::to_string(recordPosition))+".data";
    QFile file(path);
    file.open(QFile::ReadOnly);
    QDataStream stream(&file);
    std::vector<double> z(numOfDoubles+numOfInetegers , 0);
    std::vector<int> zi(numOfInetegers , 0);
    std::vector<double> zd(numOfDoubles , 0);
    emit beginResetModel();
    m_table.clear();
    m_table.resize(file.size()/bytesInOneRow);
    rowNum = file.size()/bytesInOneRow;
    while(file.pos()!=file.size())
    {
        for (int i=0;i<numOfInetegers;i++)
        {
            stream>>zi[i];
//            zi[i] = zi[0];
        }
        stream.setFloatingPointPrecision(QDataStream::DoublePrecision);
        for (int i=0;i<numOfDoubles;i++)
        {
            stream>>zd[i];
//            zd[i] = zi[0];
        }
        for (int i=0;i<numOfDoubles+numOfInetegers;i++)
        {
            (i>=numOfInetegers) ? z[i] = zd[i-numOfInetegers] : z[i] = zi[i];
        }
        m_table[n_col] = z;
        m_table[n_col].resize(z.size());
        colNum = z.size();
        n_col++;
    }
    emit endResetModel();
    file.close();
    return;
}
int TableModel::sii()
{
    return rowNum+colNum;
}
void TableModel::sort(int column, Qt::SortOrder order)
{
    emit beginResetModel();
    if (order)std::stable_sort(m_table.begin() , m_table.end() ,[column](std::vector<double> a , std::vector<double> b) -> bool {if (a[column]>b[column]){return false;}return true;});
    else std::stable_sort(m_table.begin() , m_table.end() , [column](std::vector<double> a , std::vector<double> b) -> bool {if (a[column]>b[column]){return true;}return false;});
    emit endResetModel();
    return;
}

