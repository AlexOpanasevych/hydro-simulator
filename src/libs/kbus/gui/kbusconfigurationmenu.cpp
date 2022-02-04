#include "kbusconfigurationmenu.h"
#include "kbusfactory.h"

KBusConfigurationMenu::KBusConfigurationMenu(QWidget *parent) : QMenuBar(parent)
{
    QMenu *busMenu = new QMenu("KBus");

    this->addMenu(busMenu);

//    this->layout()->setMenuBar(menu);

    QList<QString> buses = KBusFactory::instance()->getCreatorsNames();

    for (auto &el: buses) {
        QAction *act = busMenu->addAction(el);
        act->setData(el);
        qDebug() << el;
//        connect(act, &QAction::triggered, this, [this, act](){
//            rxBus = KBusConfigurationWindget(act);
//        } );
    }
}
