#include <LayerShellQt/shell.h>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "xdgimageprovider.h"
#include "plugin-taskbar.h"

int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    LayerShellQt::Shell::useLayerShell();

    Taskbar taskbar;
    ForeignToplevelManager manager(&taskbar);

    QQmlApplicationEngine engine;

    // Register the image provider for icons
    XdgImageProvider XdgImageProvider;
    engine.addImageProvider("icon", &XdgImageProvider);

    // Expose taskbar model to QML
    engine.rootContext()->setContextProperty("taskbar", &taskbar);
    engine.load(QUrl("qrc:/main.qml"));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
