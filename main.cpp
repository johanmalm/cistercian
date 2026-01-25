#include <QGuiApplication>
#include <QTimer>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "plugin-taskbar.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    Taskbar taskbar;
    ForeignToplevelManager manager(&taskbar);

    QQmlApplicationEngine engine;

    // Expose taskbar model to QML
    engine.rootContext()->setContextProperty("taskbar", &taskbar);
    engine.load(QUrl("qrc:/main.qml"));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
