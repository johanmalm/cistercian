#ifndef ICONPROVIDER_H
#define ICONPROVIDER_H

#include <QObject>
#include <QString>
#include <QIcon>
#include <XdgIcon>

class IconProvider : public QObject {
    Q_OBJECT
public:
    explicit IconProvider(QObject *parent = nullptr);

    Q_INVOKABLE QString getIconPath(const QString &iconName, int size = 48);
    Q_INVOKABLE bool hasIcon(const QString &iconName);
};

#endif // ICONPROVIDER_H
