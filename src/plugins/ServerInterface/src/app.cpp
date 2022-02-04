#include "app.h"

App::App(QObject *parent) : QObject(parent)
{
}

void App::initConfigurator(QString appPath)
{
     m_sConfigurator = new ServerConfigurator(m_server, appPath, this);
}
