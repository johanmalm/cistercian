#ifndef XDGIMAGEPROVIDER_H
#define XDGIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QPixmap>

class XdgImageProvider : public QQuickImageProvider {
public:
    XdgImageProvider();
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize) override;
};

#endif // XDGIMAGEPROVIDER_H
