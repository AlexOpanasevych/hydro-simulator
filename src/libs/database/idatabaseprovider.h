#ifndef IDATABASEPROVIDER_H
#define IDATABASEPROVIDER_H

#include <QString>
#include <QVariant>
#include <QVariantMap>
#include <database_global.h>


//!
//! \brief The IDatabaseProvider class
//!
class DATABASE_EXPORT IDatabaseProvider
{
    QString m_tableName;
public:
    IDatabaseProvider(){}

    enum class JoinType {INNER, LEFT, RIGHT, FULL};
    enum class ConditionType {WHERE, SELECT, INITSTATE};

    //!
    //! \brief setTable
    //! \param name
    //!
    void setTable(QString name) {m_tableName = name;}

    //!
    //! \brief getRow
    //! \param index
    //! \return
    //!
    virtual QVariant getRow(int index) = 0;

    //!
    //! \brief getRowField
    //! \param fieldName
    //! \param fieldValue
    //! \return
    //!
    virtual QVariant getRowField(const QString & fieldName, const QVariant & fieldValue) = 0;

    //!
    //! \brief getRowField
    //! \param fieldName
    //! \param fieldValue
    //! \param index
    //! \return
    //!
    virtual QVariant getRowField(const QString & fieldName, const QVariant & fieldValue, int index) = 0;

    //!
    //! \brief Select function that wraps SELECT statement
    //! \param field
    //! \return \a *this reference to itself
    //!
    virtual IDatabaseProvider & select(const QList<QString> &field = {}) = 0;

    //!
    //! \brief del
    //! \return \a *this reference to itself
    //!
    virtual IDatabaseProvider & del() = 0;

    //!
    //! \brief INSERT statement wrapper
    //! \param values
    //! \return \a true if succeed, otherwise \a false
    //!
    virtual bool insertRow(const QMap<QString, QVariant> & values) = 0;
    //!
    //! \brief UPDATE state wrapper
    //! \param data
    //! \param value
    //! \return \a true if succeed, otherwise \a false
    //!
    virtual bool updateRow(const QVariantMap &data, const QPair<QString, QVariant> &value) = 0;

    //!
    //! \brief Get last ID in current table
    //! \return \a id of last record in table
    //!
    virtual int getLastID() = 0;

    //!
    //! \brief SELECT JOIN statement wrapper
    //! \param fieldNames
    //! \param secondTable
    //! \param type
    //! \param foreignKey
    //! \param referencedField
    //! \return \a *this reference
    //!
    virtual IDatabaseProvider & selectJoin(const QList<QString> &fieldNames, const QString &secondTable, JoinType type, QString foreignKey, QString referencedField) = 0;

    //!
    //! \brief Executes query and returns result
    //! \return list of records
    //!
    virtual QList<QVariantMap> get() = 0;

    //!
    //! \brief WHERE statement, don't forget to execute get()
    //! \param col
    //! \return \a *this reference, we can use where again (or whereIn and whereNotIn)
    //!
    virtual IDatabaseProvider & where(const QPair<QString, QVariant> & col) = 0;

    //!
    //! \brief WHERE IN wrapper, don't forget to execute get()
    //! \param field
    //! \param values
    //! \return \a *this reference, we can use whereIn again (or where and whereNotIn)
    //!
    virtual IDatabaseProvider & whereIn(const QString field, const QVariantList & values) = 0;

    //!
    //! \brief WHERE NOT IN wrapper, don't forget to execute get()
    //! \param field
    //! \param values
    //! \return \a *this reference, we can use whereIn again (or where and whereNotIn)
    //!
    virtual IDatabaseProvider & whereNotIn(const QString field, const QVariantList & values) = 0;

    //!
    //! \brief destructor for IDatabaseProvider
    //!
    virtual ~IDatabaseProvider(){};

    //!
    //! \brief Gets table name
    //! \return current table name
    //!
    QString getTableName() const {return m_tableName;}

protected: // internal
};

#endif // IDATABASEPROVIDER_H


