#include "bronze.h"

#include <src/abstractfactory.h>
#include <PythonQt.h>
#include <PythonQt_QtAll.h>
#include <cdebug.h>

Bronze::Bronze(QObject *parent) : Station("bronze", parent)
{
    connect(this, &Bronze::appPathChanged, this, &Bronze::updatePythonLogic);

    QFile file(appPath() + filePath);
    if(!file.open(QFile::ReadOnly)) {
        qDebug(s) << "not opened";
    }
    else {
        qDebug(s) << "opened";
    }
    PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
    PythonQt_QtAll::init();
//    PythonQt::self()->registerCPPClass("BronzeButton", "", "mgk", PythonQtCreateObject<MGK345Wrapper>);
//    PythonQt::self()->registerCPPClass("BronzeNumberView", "", "mgk", PythonQtCreateObject<MGK345Wrapper>);

    mainContext = PythonQt::self()->getMainModule();
    for(auto & resource : this->m_statesList) {
        if(!resource->name().isEmpty()) {
            if(resource->controlType() == "Button")
                PythonQt::self()->addObject(mainContext, resource->name(), new BronzeButton(resource->name(), resource, this));
            else if(resource->controlType() == "NumberView")
                PythonQt::self()->addObject(mainContext, resource->name(), new BronzeNumberView(resource->name(), resource, this));
        }
    }
    console = new PythonQtScriptingConsole(NULL, mainContext);
//    mainContext.evalFile(appPath() + filePath);
    if(mainContext.isNull()) {
        qDebug(s) << "Context not init, path:" << m_appPath + filePath;
    }
    else {

        qDebug(s) << "Context init, path:" << m_appPath + filePath;
    }
    console->show();

    AbstractFactory::registerItem<BronzeTransmitter>();
    AbstractFactory::registerItem<BronzeReceiver>();
    transmitter = dynamic_cast<BronzeTransmitter*>(AbstractFactory::produceItem("BronzeTransmitter"));
    receiver = dynamic_cast<BronzeReceiver*>(AbstractFactory::produceItem("BronzeReceiver"));
    addSubsystem(transmitter);
    addSubsystem(receiver);


    connect(this, &Bronze::pushState, this, [this](const StateData & stData){
        this->setCurrentClient(stData.clientName);
        qDebug(s) << "bronze:" << stData.resourceName << stData.value;
        if(stData.value.canConvert<QVariantMap>()) {
            QVariantMap dat = stData.value.toMap();
            mainContext.call("on_" + stData.resourceName + "_" + "changed", {}, {{"value", dat["value"]}});
        }
        else mainContext.call("on_" + stData.resourceName + "_" + "changed", {stData.value});
    });

}

void Bronze::updatePythonLogic()
{
    QFile file(m_appPath + filePath);
    if(!file.open(QFile::ReadOnly)) {
        qDebug(s) << "not opened";
    }
    else {
        qDebug(s) << "opened";
        mainContext.evalScript(file.readAll());
    }
    if(mainContext.isNull()) {
        qDebug(s) << "Context not updated, path:" << m_appPath + filePath;
    }
    else {
        qDebug(s) << "Context updated, path" << m_appPath + filePath;
    }

//    mainContext.call("on_gl_pressed");
}

MGK345Wrapper::MGK345Wrapper(Bronze * station) : station(station) {}

BronzeButton * MGK345Wrapper::new_BronzeButton(const QString & name) {
    auto btn = new BronzeButton(name);
//    connect(btn, &BronzeButton::ledChanged, this, [this](int led){station->m_statesList});
    return btn;
}

BronzeButton::BronzeButton(const QString &name, QObject *parent) : BronzeEntity(name, parent)
{

}

BronzeButton::BronzeButton(const QString &name, Resource *resource, QObject *parent) : BronzeEntity(name, resource, parent) {
    connect(this, &BronzeButton::ledChanged, resource, [resource](int led){
        resource->onPropertyChanged("led", led);
    });
}

BronzeEntity::BronzeEntity(const QString &name, QObject *parent) : QObject(parent)
{
    setName(name);
}

BronzeEntity::BronzeEntity(const QString &name, Resource *resource, QObject *parent) : QObject(parent)
{
    setName(name);
    setResource(resource);
}

QVariant MGK345Wrapper::get(BronzeEntity *entity, const QString &propName)
{
    return entity->property(propName.toUtf8().constData());
}

void MGK345Wrapper::set(BronzeEntity *entity, const QString &propName, const QVariant &value)
{
    entity->setProperty(propName.toUtf8().constData(), value);
}

BronzeNumberView::BronzeNumberView(const QString &name, QObject *parent) : BronzeEntity(name, parent)
{

}

BronzeNumberView::BronzeNumberView(const QString &name, Resource *resource, QObject *parent) : BronzeEntity(name, resource, parent)
{
    connect(this, &BronzeNumberView::valueChanged, resource, [resource](double value){
        resource->onPropertyChanged("value", value);
    });
}
