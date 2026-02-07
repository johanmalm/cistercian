#include <LayerShellQt/shell.h>
#include <QGuiApplication>
//#include <QTimer>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "iconprovider.h"
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
    engine.addImageProvider("icon", new XdgImageProvider());

    // Expose icon provider to QML
    IconProvider iconProvider;
    engine.rootContext()->setContextProperty("iconProvider", &iconProvider);

    // Expose taskbar model to QML
    engine.rootContext()->setContextProperty("taskbar", &taskbar);
    engine.load(QUrl("qrc:/main.qml"));

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
