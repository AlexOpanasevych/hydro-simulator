#ifndef LOGINSERVICE_H
#define LOGINSERVICE_H

#include <QObject>
#include <QString>
#include "sqliteprovider.h"

struct DATABASE_EXPORT User {
    QString name;
    int uid;
};

struct DATABASE_EXPORT Session {
    User user;
    int sessionId;
};

class DATABASE_EXPORT LoginService : public QObject
{
    Q_OBJECT
public:
    explicit LoginService(QObject *parent = nullptr);

    //!
    //! \brief Login operation, that goes in table, searches for \p username, and if \p password
    //! \param username
    //! \param pass
    //! \return \c true if succeed, otherwise \c false
    //!
    Q_INVOKABLE bool login(QString username, QString pass);

    //!
    //! \brief Log out current user
    //! \return \c true if succeed, otherwise \c false
    //!
    Q_INVOKABLE bool logout();

    //!
    //! \brief Register new user with \a newUserName if it not exists in \a users table and password \a pass
    //! \param newUserName
    //! \param pass
    //! \return \c true if succeed, otherwise \c false
    //!
    Q_INVOKABLE bool signup(QString newUserName, QString pass);

    //!
    //! \brief Changes password \p oldPassword if user \p user is present in table
    //! \param user
    //! \param oldPassword
    //! \param newPassword
    //! \return \c true if succeed, otherwise \c false
    //!
    Q_INVOKABLE bool changePassWord(QString user, QString oldPassword, QString newPassword);

    bool createSession();
    User user();
    int sessionID;
    int userID;

signals:

private:
    SqliteProvider * dProvider = nullptr;
    bool logged = false;
    User m_user;
};

#endif // LOGINSERVICE_H
