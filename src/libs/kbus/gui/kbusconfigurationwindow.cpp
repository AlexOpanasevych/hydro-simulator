#include "kbusconfigurationwindow.h"
#include <base/kbus.h>
#include "kbusfactory.h"
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QPushButton>

KBusConfigurationWindow::KBusConfigurationWindow(QString name, QString kbusType, QWidget *parent) : QFrame(parent)
{
    QList<KBus::ConnectionData> init = KBusFactory::instance()->getBusCreateData(kbusType);

    auto configWindow = new QWidget(this);

    configWindow->setMinimumWidth(200);
    configWindow->setWindowTitle("KBus config");
    configWindow->setWindowFlags(Qt::WindowCloseButtonHint);
    this->name = name;

    QFormLayout *layout = new QFormLayout(this);
    layout->setSpacing(10);

    requestData.clear();

    for (auto &el: init) {
        if (el.type == "MainLabel") {
            QLabel *mainLabel = new QLabel(this->name);
            mainLabel->setAlignment(Qt::AlignHCenter);
            QFont titleLabelFont("Arial", 10, QFont::Bold);
            mainLabel->setFont(titleLabelFont);
            layout->addRow(mainLabel);
        } else if (el.type == "Label") {
            QLabel *label = new QLabel(el.name);
            layout->addRow(label);
        } else if (el.type == "ComboBox") {
            QLabel *label = new QLabel(el.name);
            QComboBox *combobox = new QComboBox();
            QStringList values = el.data.toStringList();
            combobox->insertItems(0, values);
            requestData.push_back({el.name, "ComboBox", combobox});
            layout->addRow(label, combobox);

            if(el.name == "Port"){
                connect(combobox, &QComboBox::currentTextChanged, [this](QString portName){emit busPortChanged(portName.replace("COM", "").toInt());});
            }
            else if(el.name == "Speed"){
                connect(combobox, &QComboBox::currentTextChanged, [this](QString speed){emit busSpeedChanged(speed.toInt());});
            }

        } else if (el.type == "TextInput"){

        } else {
            VDebug::Warning("Element type not handlered!");
        }
    }

    for (auto &el: requestData) {
        if (el.type == "ComboBox") {
            QComboBox *cb = dynamic_cast<QComboBox*>(el.widget);

            if(el.name == "Port"){
                for(int ind = 0; ind < cb->count(); ind++){
                    if(cb->itemText(ind) == ("COM" + QString::number(busParams.port))){
                        cb->setCurrentIndex(ind);
                    }
                }
            }
            else if(el.name == "Speed"){
                for(int ind = 0; ind < cb->count(); ind++){
                    if(cb->itemText(ind) == QString::number(busParams.speed)){
                        cb->setCurrentIndex(ind);
                    }
                }
            }
        }
    }

    QPushButton *connectButton = new QPushButton("Connect");
    connect(connectButton, &QPushButton::clicked, this, [&](){
        QVariantMap data;
        for (auto &el: requestData) {
            if (el.type == "ComboBox") {
                QComboBox *cb = dynamic_cast<QComboBox*>(el.widget);
                QString value = cb->currentText();

                data.insert(el.name, value);
            }
        }

        emit connectButtonClicked(data);
    });

    layout->addRow(connectButton);
    QPushButton *disconnectButton = new QPushButton("Disconnect");
    connect(disconnectButton, &QPushButton::clicked, this, [&](){
        emit disconnectButtonClicked();
    });

    layout->addRow(disconnectButton);

    this->setLayout(layout);
    this->setFrameShape(QFrame::Box);

    this->layout()->setSpacing(10);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void KBusConfigurationWindow::setDefaultBusParams(BusParams params){
    busParams = params;
}
