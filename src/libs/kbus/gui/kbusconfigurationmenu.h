#ifndef KBUSCONFIGURATIONMENU_H
#define KBUSCONFIGURATIONMENU_H

#include <QMenuBar>
#include <QWidget>

class KBusConfigurationMenu : public QMenuBar
{
    Q_OBJECT
public:
    KBusConfigurationMenu(QWidget *parent = nullptr);
    ~KBusConfigurationMenu(){};

signals:

};

#endif // KBUSCONFIGURATIONMENU_H
