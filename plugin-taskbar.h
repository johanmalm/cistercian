#pragma once
#include <QAbstractListModel>
#include <QObject>
#include <QWaylandClientExtension>
#include <functional>
#include "qwayland-wlr-foreign-toplevel-management-unstable-v1.h"

class Toplevel : public QObject
{
    Q_OBJECT

    // Q_PROPERTY(QString title MEMBER m_title NOTIFY titleChanged)
    Q_PROPERTY(QString title READ title NOTIFY titleChanged)
    Q_PROPERTY(bool activated READ activated NOTIFY activatedChanged)
    Q_PROPERTY(QString appId READ appId NOTIFY appIdChanged)

public:
    explicit Toplevel(QObject *parent = nullptr);

    void setTitle(const QString &title);
    void setActivated(bool activated);
    void setAppId(const QString &appId);

    // Call this to activate the window
    Q_INVOKABLE void activate();
    void setActivateCallback(std::function<void()> cb);

signals:
    void activatedChanged();
    void titleChanged();
    void appIdChanged();

private:
    QString m_title;
    bool m_activated;
    QString m_appId;
    std::function<void()> m_activateCallback;

    // Getters
public:
    QString title() const { return m_title; }
    bool activated() const { return m_activated; }
    QString appId() const { return m_appId; }
};

class Taskbar : public QAbstractListModel
{
    Q_OBJECT

public:
    enum Roles {
        ToplevelRole = Qt::UserRole + 1,
        TitleRole,
        ActivatedRole,
        ObjectRole,
    };

    explicit Taskbar(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void addToplevel(Toplevel *toplevel);
    void removeToplevel(Toplevel *toplevel);

private:
    QList<Toplevel *> m_toplevels;
};

// Derive from QtWayland::zwlr_foreign_toplevel_handle_v1 so that we can call init() etc directly.
class ForeignToplevelHandle : public QObject, public QtWayland::zwlr_foreign_toplevel_handle_v1
{
    Q_OBJECT
public:
    ForeignToplevelHandle(struct ::zwlr_foreign_toplevel_handle_v1 *handle,
                          QObject *parent = nullptr);
    ~ForeignToplevelHandle();

    void activate();

protected:
    void zwlr_foreign_toplevel_handle_v1_title(const QString &title);
    void zwlr_foreign_toplevel_handle_v1_app_id(const QString &app_id);
    void zwlr_foreign_toplevel_handle_v1_state(wl_array *state);
    void zwlr_foreign_toplevel_handle_v1_done();
    void zwlr_foreign_toplevel_handle_v1_closed();

private:
    struct ::zwlr_foreign_toplevel_handle_v1 *m_handle = nullptr;
    Toplevel *m_toplevel = nullptr;

    // Getters
public:
    Toplevel *toplevel() const { return m_toplevel; }
};

class ForeignToplevelManager : public QWaylandClientExtensionTemplate<ForeignToplevelManager>,
                               public QtWayland::zwlr_foreign_toplevel_manager_v1
{
    Q_OBJECT

public:
    explicit ForeignToplevelManager(Taskbar *taskbar);

protected:
    void zwlr_foreign_toplevel_manager_v1_toplevel(
            struct ::zwlr_foreign_toplevel_handle_v1 *handle) override;

private slots:
    void onActiveChanged();

private:
    Taskbar *m_taskbar;
};
