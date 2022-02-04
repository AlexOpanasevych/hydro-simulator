#ifndef SQLITEPROVIDER_H
#define SQLITEPROVIDER_H


#include "idatabaseprovider.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>

#include <QVariantMap>

class DATABASE_EXPORT SqliteProvider : public IDatabaseProvider
{
private:
    QSqlDatabase m_database;
    QVariantMap whereConditions;
    QString queryString;
    ConditionType cond = ConditionType::INITSTATE;
    int lastInID = 0;
public:
    SqliteProvider(QString databaseName);
    QVariant getRow(int index) override;
    QVariant getRowField(const QString & fieldName, const QVariant & fieldValue) override;
    QVariant getRowField(const QString & fieldName, const QVariant & fieldValue, int index) override;
    SqliteProvider & select(const QList<QString> &field = {}) override;
    bool insertRow(const QMap<QString, QVariant> & values) override;
    bool updateRow(const QVariantMap &data, const QPair<QString, QVariant> &value) override;
    int getLastID() override;
    SqliteProvider & selectJoin(const QList<QString> &fieldNames, const QString &secondTable, JoinType type, QString foreignKey, QString referencedField) override;

    bool updateInsert(const QVariantMap &insertConditions,
                      const QVariantMap &insert,
                      const QVariantMap &update,
                      const QVariantMap &whereUpdate) const;

    QList<QVariantMap> get() override;
    SqliteProvider & where(const QPair<QString, QVariant> &col) override;
    SqliteProvider & whereIn(const QString field, const QVariantList &values) override;
    SqliteProvider & whereNotIn(const QString field, const QVariantList &values) override;
    SqliteProvider & del() override;
};


//class Table {
//    QSqlQuery
//    public:
//        Row getRow();
//}

#endif // DATABASEPROVIDER_H
