import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import org.kde.layershell 1.0 as LayerShell

Window {
    id: panelWindow
    visible: true
    color: "transparent"
    width: Screen.width
    height: 48

    LayerShell.Window.scope: "dock"
    LayerShell.Window.anchors: LayerShell.Window.AnchorBottom
    LayerShell.Window.layer: LayerShell.Window.LayerTop
    LayerShell.Window.exclusionZone: height

    Rectangle {
        anchors.fill: parent
        anchors.margins: 4
        radius: 6
        color: "#80222222"
        border.width: 1
        border.color: "#A0222222"

        // The RowLayout contains the plugins
        // On a technical note, the RowLayout allows Layout.* stuff to be used
        RowLayout {
            id: panelRow
            anchors.fill: parent
            anchors.margins: 4
            spacing: 4

            // Taskbar Plugin
            // TODO: put this in a separate file like plugin-taskbar.qml
            Repeater {
                model: taskbar

                delegate: Rectangle {
                    id: task
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Layout.minimumWidth: 48
                    Layout.maximumWidth: 140

                    radius: 4
                    color: "#20000000"

                    border.width: 1
                    border.color: toplevel.activated ? "#FF888888" : "#00000000"

                    // Task Layout
                    RowLayout {
                        anchors.fill: parent
                        anchors.margins: 4
                        spacing: 6

                        // Icon
                        Image {
                            Layout.preferredWidth: 24
                            Layout.preferredHeight: 24
                            Layout.alignment: Qt.AlignVCenter
                            source: toplevel.appId ? "image://icon/" + toplevel.appId + "/24" : "image://icon/application-x-executable/24"
                            smooth: true
                        }

                        Text {
                            Layout.fillWidth: true
                            Layout.alignment: Qt.AlignVCenter
                            verticalAlignment: Text.AlignVCenter
                            horizontalAlignment: Text.AlignHLeft
                            elide: Text.ElideRight
                            wrapMode: Text.NoWrap
                            text: toplevel.title
                            font.pixelSize: 14
                            color: "white"
                        }
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            toplevel.activate()
                        }
                    }
                }
            }

            // Expansion-spacer to pack all tasks to the left
            Item {
                Layout.fillWidth: true
            }

            // Load plugins
            Repeater {
                id: pluginRepeater
                model: plugins

                delegate: Loader {
                    Layout.fillHeight: true
                    source: pluginSource
                    onLoaded: {
                        if (item) {
                            item.parent = parent
                        }
                    }
                }
            }

        }
    }

    ListModel {
        id: plugins
        ListElement { pluginSource: "plugin-clock.qml" }
    }
}
