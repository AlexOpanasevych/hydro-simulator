#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "testobject.h"
#include "tablemanager.h"
#include <QQmlContext>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    TableManager::instance();
    ui->setupUi(this);
    qmlRegisterType<QObject>("KLib", 1, 0, "Null");
    ui->quickWidget->setSource(QUrl("qrc:/main.qml"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

