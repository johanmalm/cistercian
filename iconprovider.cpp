#include "iconprovider.h"
#include <QIcon>
#include <XdgIcon>

IconProvider::IconProvider(QObject *parent) : QObject(parent) {
}

QString IconProvider::getIconPath(const QString &iconName, int size) {
    if (iconName.isEmpty()) {
        return QString();
    }

    return QString("image://icon/%1/%2").arg(iconName).arg(size);
}

bool IconProvider::hasIcon(const QString &iconName) {
    return !XdgIcon::fromTheme(iconName).isNull();
}
