#include <QGuiApplication>
#include <QQmlApplicationEngine>

// #include <QtQml/QQmlExtensionPlugin>
// Q_IMPORT_QML_PLUGIN(ExtraModulePlugin)

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"./main.qml"_qs);
    engine.load(url);

    return app.exec();
}