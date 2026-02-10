#include "plugin-taskbar.h"
#include <QGuiApplication>

Toplevel::Toplevel(QObject *parent) : QObject(parent), m_activated(false) { }

void Toplevel::setTitle(const QString &title)
{
    if (m_title == title) {
        return;
    }
    m_title = title;
    emit titleChanged();
}

void Toplevel::setActivated(bool activated)
{
    if (m_activated == activated) {
        return;
    }
    m_activated = activated;
    emit activatedChanged();
}

void Toplevel::setAppId(const QString &appId)
{
    if (m_appId == appId) {
        return;
    }
    m_appId = appId;
    emit appIdChanged();
}

void Toplevel::activate()
{
    if (m_activateCallback) {
        m_activateCallback();
    }
}

void Toplevel::setActivateCallback(std::function<void()> cb)
{
    // m_activateCallback = cb;
    m_activateCallback = std::move(cb);
}

Taskbar::Taskbar(QObject *parent) : QAbstractListModel(parent) { }

int Taskbar::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_toplevels.size();
}

QVariant Taskbar::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_toplevels.size()) {
        return {};
    }
    auto *toplevel = m_toplevels.at(index.row());
    switch (role) {
    case ToplevelRole:
        return QVariant::fromValue(toplevel);
    case TitleRole:
        return toplevel->title();
    case ActivatedRole:
        return toplevel->activated();
    case ObjectRole:
        return QVariant::fromValue(toplevel);
    }
    return {};
}

// roleNames() defines how data roles in the C++ model are exposed to QML
// In the C++ the enums get used, whereas QML can only the role name strings
// TODO: Use title etc is main.qml
QHash<int, QByteArray> Taskbar::roleNames() const
{
    return { { ToplevelRole, "toplevel" },
             { TitleRole, "title" },
             { ActivatedRole, "activated" },
             { ObjectRole, "toplevel" } };
}

void Taskbar::addToplevel(Toplevel *toplevel)
{
    beginInsertRows(QModelIndex(), m_toplevels.size(), m_toplevels.size());
    m_toplevels.append(toplevel);
    endInsertRows();
}

void Taskbar::removeToplevel(Toplevel *toplevel)
{
    int index = m_toplevels.indexOf(toplevel);
    if (index < 0) {
        return;
    }
    beginRemoveRows(QModelIndex(), index, index);
    m_toplevels.removeAt(index);
    endRemoveRows();
}

ForeignToplevelHandle::ForeignToplevelHandle(struct ::zwlr_foreign_toplevel_handle_v1 *handle,
                                             QObject *parent)
    : QObject(parent),
      QtWayland::zwlr_foreign_toplevel_handle_v1(handle),
      m_toplevel(new Toplevel(this))
{
    m_handle = handle;
    // Optional: hook signals from m_handle to update m_toplevel->activated
}

ForeignToplevelHandle::~ForeignToplevelHandle()
{
    delete m_toplevel;
}

void ForeignToplevelHandle::activate()
{
    if (!m_handle) {
        qDebug() << "activate() called by no m_handle";
        return;
    }
    auto waylandApp = qGuiApp->nativeInterface<QNativeInterface::QWaylandApplication>();
    wl_seat *seat = waylandApp->seat();
    if (!seat) {
        qDebug() << "activate() called but wl_seat is null";
        return;
    }
    zwlr_foreign_toplevel_handle_v1_activate(m_handle, seat);
}

void ForeignToplevelHandle::zwlr_foreign_toplevel_handle_v1_title(const QString &title)
{
    m_toplevel->setTitle(title);
}

void ForeignToplevelHandle::zwlr_foreign_toplevel_handle_v1_app_id(const QString &app_id)
{
    m_toplevel->setAppId(app_id);
}

void ForeignToplevelHandle::zwlr_foreign_toplevel_handle_v1_state(wl_array *state)
{
    bool activated = false;

    auto *data = static_cast<uint32_t *>(state->data);
    size_t count = state->size / sizeof(uint32_t);

    for (size_t i = 0; i < count; ++i) {
        if (data[i] == ZWLR_FOREIGN_TOPLEVEL_HANDLE_V1_STATE_ACTIVATED)
            activated = true;
    }

    m_toplevel->setActivated(activated);
}

void ForeignToplevelHandle::zwlr_foreign_toplevel_handle_v1_done()
{
    // State batch complete — no-op for now
}

void ForeignToplevelHandle::zwlr_foreign_toplevel_handle_v1_closed()
{
    deleteLater();
}

ForeignToplevelManager::ForeignToplevelManager(Taskbar *taskbar)
    : QWaylandClientExtensionTemplate<ForeignToplevelManager>(3), m_taskbar(taskbar)
{
    connect(this, &QWaylandClientExtension::activeChanged, this,
            &ForeignToplevelManager::onActiveChanged);

    initialize();
}

void ForeignToplevelManager::zwlr_foreign_toplevel_manager_v1_toplevel(
        struct ::zwlr_foreign_toplevel_handle_v1 *handle)
{
    auto *wrapper = new ForeignToplevelHandle(handle, this);
    Toplevel *toplevel = wrapper->toplevel();
    toplevel->setActivateCallback([wrapper]() { wrapper->activate(); });
    m_taskbar->addToplevel(toplevel);
    connect(wrapper, &QObject::destroyed, this,
            [this, toplevel]() { m_taskbar->removeToplevel(toplevel); });
}

void ForeignToplevelManager::onActiveChanged()
{
    // no-op
}
