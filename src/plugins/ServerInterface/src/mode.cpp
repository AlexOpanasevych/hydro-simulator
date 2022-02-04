#include "mode.h"

Mode::Mode(QObject *parent) : QObject(parent)
{
    setClients(m_clientList.model());
}

KObservableList<Client> &Mode::clientList()
{
    return m_clientList;
}

void Mode::setClientList(const KObservableList<Client> &clientList)
{
    m_clientList = clientList;
}

void Mode::addClient(Client *client)
{
    m_clientList.push_back(client);
}

void Mode::removeClient(int index)
{
    m_clientList.remove(index);
}

int Mode::clientsCount()
{
    return m_clientList.count();
}

void Mode::removeClient(Client * cl)
{
    m_clientList.remove(cl);
}
