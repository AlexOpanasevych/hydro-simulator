#ifndef MODE_H
#define MODE_H

#include <QObject>
#include <kmacro.h>
#include <utility/kobservablelist.h>
#include "client.h"

class SERVERINTERFACE_LIBRARY_EXPORT Mode : public QObject
{
    Q_OBJECT
    K_QML_TYPE(Mode)
    K_AUTO_PROPERTY(KObservableModel*, clients, clients, setClients, clientsChanged, nullptr)
    K_AUTO_PROPERTY(QString, name, name, setName, nameChanged, "")
    K_AUTO_PROPERTY(int, x, x, setX, xChanged, 0)
    K_AUTO_PROPERTY(int, y, y, setY, yChanged, 0)
    K_AUTO_PROPERTY(int, width, width, setWidth, widthChanged, 0)
    K_AUTO_PROPERTY(int, height, height, setHeight, heightChanged, 0)
public:
    explicit Mode(QObject *parent = nullptr);

    //!
    //! \brief Get client list
    //! \return list of clients
    //!
    KObservableList<Client> & clientList();

    //!
    //! \brief Sets new client list
    //! \param clientList
    //!
    void setClientList(const KObservableList<Client> &clientList);

    //!
    //! \brief Adds client to list
    //! \param client
    //!
    void addClient(Client *client = nullptr);

    //!
    //! \brief Removes client by given \p index and (if is bound) unbinds it from mode
    //! \param index
    //!
    void removeClient(int index);

    //!
    //! \brief Get clients count
    //! \return clients count
    //!
    int clientsCount();

    //!
    //! \brief Removes client from list and (if is bound) unbinds from mode
    //!
    void removeClient(Client *);


signals:
private:
    KObservableList<Client> m_clientList;
};

#endif // SERVER_H
