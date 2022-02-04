#include <QApplication>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QQuickView>
#include <engine.h>

#include <scene/kscene.h>
#include <scene/ksceneprovider.h>
#include <tools/devlogger.h>
#include <tools/pluginsmanager.h>
#include <QLoggingCategory>
#include <cdebug.h>
#include "PythonQt.h"
#include "PythonQt_QtAll.h"
#include "gui/PythonQtScriptingConsole.h"

static const QtMessageHandler QT_DEFAULT_MESSAGE_HANDLER = qInstallMessageHandler(0);

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString & msg)
{
    QString user = qEnvironmentVariable("DEV");
    if (user == "") (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
    if (!msg.contains("Error: A QmlLoggingCatgory was provided without a valid name")){
        if (qstrcmp(context.category, "global") == 0 ){
            DevLogger::instance()->addLog(context.category, context.function, msg);
            (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
        } else if(qstrcmp(context.category, "log") == 0) {
            qDebug(g) << "adadadadaadaadadadadadadadadaad";
            DevLogger::instance()->addFancyLog(context.category, context.function, msg);
        } else if (user.contains(context.category)){
            (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
//            DevLogger::instance()->addLog(context.category, context.function, msg);
        }else if (qstrcmp(context.category, "all") == 0){
            (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
        }else if (type == QtMsgType::QtWarningMsg
                  || type == QtMsgType::QtFatalMsg
                  || type == QtMsgType::QtCriticalMsg){
            (*QT_DEFAULT_MESSAGE_HANDLER)(type, context, msg);
        }
    }
}

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    if (qEnvironmentVariable("DEV") != "")
    {
        qSetMessagePattern("%{if-category}\033[32m%{category}:%{endif}%{if-warning}\033[37m%{file}:%{line}: %{endif}\033\[31m%{if-debug}\033\[36m%{endif}[%{function}]\033[0m %{message}");
        qInstallMessageHandler(customMessageHandler);
    }

    QApplication app(argc, argv);
    //! [for qml file dialog]
    app.setOrganizationName("...");
    app.setOrganizationDomain("...");
    app.setApplicationName("Simulator");
    app.setApplicationDisplayName("Simulator");
    //! [for qml file dialog]
    qDebug() << "App path : " << qApp->applicationDirPath();

    //! [app]
    QQmlApplicationEngine appEngine;

//    appEngine.setObjectOwnership(DevLogger::instance(), QQmlEngine::CppOwnership);
    //! [app]
    auto coreEngine = new Engine();
    auto engine = static_cast<QQmlEngine*>(&appEngine);

    PluginsManager::instance()->setAppPath(argv[0]);
    PluginsManager::instance()->setEngine(engine);
    PluginsManager::instance()->setScene(KSceneProvider::instance()->getScene("first"));
    PluginsManager::instance()->setCoreEngine(coreEngine);
    PluginsManager::instance()->load();

    //! [additional]

    coreEngine->start();
    QObject::connect(&app, &QGuiApplication::aboutToQuit, coreEngine, [coreEngine](){
        coreEngine->stop();
        delete coreEngine;
    });

    //antialiasing
    QSurfaceFormat format;
    format.setSamples(8);
    QSurfaceFormat::setDefaultFormat(format);

    auto ei = coreEngine->getInterface();
    qmlRegisterType<QObject>("KLib", 1, 0, "Null");
        qDebug(v) << engine->importPathList();
    KScene * kscene = KSceneProvider::instance()->getScene("first");
    kscene->setEngineInterface(ei);

    const QUrl url(QStringLiteral("qrc:/resources/qml/main.qml"));
    QObject::connect(&appEngine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    appEngine.load(url);

//    PythonQt::init(PythonQt::IgnoreSiteModule | PythonQt::RedirectStdOut);
//    PythonQt_QtAll::init();

////    PythonQt::self()->registerCPPClass("BronzeButton", "BronzeEntity", "mgk"/*, PythonQtCreateObject<MGK345Wrapper>*/);

//    PythonQtObjectPtr  mainContext = PythonQt::self()->getMainModule();
//    PythonQtScriptingConsole console(NULL, mainContext);
////    mainContext.evalFile(filePath);
//    if(mainContext.isNull()) {
//        qDebug(s) << "Context not init, path:"/* << m_appPath + filePath*/;
//    }
//    console.show();



    //! [additional]
    return app.exec();
}
