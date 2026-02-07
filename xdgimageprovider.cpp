#include "xdgimageprovider.h"
#include <XdgIcon>
#include <QIcon>

XdgImageProvider::XdgImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Pixmap) {
}

QPixmap XdgImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) {
    // id format: "iconName/size" or just "iconName"
    QStringList parts = id.split('/');
    QString iconName = parts.value(0);
    int iconSize = parts.value(1, "48").toInt();

    if (requestedSize.isValid()) {
        iconSize = qMax(requestedSize.width(), requestedSize.height());
    }

    QIcon icon = XdgIcon::fromTheme(iconName);

    if (icon.isNull()) {
        icon = XdgIcon::fromTheme("application-x-executable");
    }

    QPixmap pixmap = icon.pixmap(iconSize, iconSize);

    if (size) {
        *size = pixmap.size();
    }

    return pixmap;
}
