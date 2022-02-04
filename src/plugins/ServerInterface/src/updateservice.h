#ifndef MYSQLUPDATESERVICE_H
#define MYSQLUPDATESERVICE_H

#include <QDir>
#include <kmacro.h>
#include <ssmsprovider.h>
#include <utility/kutilinfo.h>
#include "serverinterface_global.h"

class SERVERINTERFACE_LIBRARY_EXPORT UpdateService : public QObject
{
    Q_OBJECT
    K_QML_TYPE(UpdateService)
public:
    explicit UpdateService(QObject * parent = nullptr);
    ~UpdateService() {}
signals:
    void pluginsUpdated();
public slots:
    //!
    //! \brief Updates plugins to actual version
    //! \param author
    //! \param version
    //! \param description
    //!
    void updatePlugins(QString author = "", int version = -1, QString description = "");
private:
    inline QList<QString> pluginPaths();
    SSMSProvider m_provider;
    QDir pluginsDirectory;
};

template<typename T>
//!
//! \brief Converts QList with template parameter type to QVariantList
//! \param list
//! \return converted QVariantList
//!
QVariantList toQVariantList(const QList<T> & list) {
    QVariantList converted;
    for (auto && item : list) {
        converted << QVariant::fromValue(item);
    }
    return converted;
}

#endif // MYSQLUPDATESERVICE_H
