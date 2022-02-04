#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <app.h>

int main(int argc, char *argv[])
{

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<QObject>("KLib", 1, 0, "");
//    qDebug() << argv[0];
//    std::string path(argv[0]);
//    auto nthSubstr = [](int n, const std::string& s,
//                  const std::string& p) -> int {
//       std::string::size_type i = s.find(p);     // Find the first occurrence

//       int j;
//       for (j = 1; j < n && i != std::string::npos; ++j)
//          i = s.find(p, i+1); // Find the next occurrence

//       if (j == n)
//         return(i);
//       else
//         return(-1);
//    };



    App::instance()->setResPath(RESOURCES_PATH);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
