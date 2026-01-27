import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.15
import org.kde.layershell 1.0 as LayerShell

Window {
    id: panelWindow
    visible: true
    color: "transparent"
    width: Screen.width
    height: 40

    LayerShell.Window.scope: "dock"
    LayerShell.Window.anchors: LayerShell.Window.AnchorBottom
    LayerShell.Window.layer: LayerShell.Window.LayerTop
    LayerShell.Window.exclusionZone: height

    Rectangle {
        anchors.fill: parent
        radius: 6
        color: "#80222222"

        // Taskbar
        //   - RowLayout allows Layout.* stuff to be used
        RowLayout {
            id: taskbarRow
            anchors.fill: parent
            anchors.margins: 4
            spacing: 4

            Repeater {
                model: taskbar

                delegate: Rectangle {
                    Layout.fillHeight: true

                    Layout.preferredWidth: 140
                    Layout.minimumWidth: 48
                    Layout.maximumWidth: taskbarRow.width / Math.max(1, taskbar.count)
                    Layout.fillWidth: true

                    width: 32
                    height: 32
                    radius: 4
                    color: toplevel.activated ? "#ffaa00" : "#555555"

                    Text {
                        anchors.centerIn: parent
                        text: toplevel.title
                        font.pixelSize: 10
                        color: "white"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            toplevel.activate()
                        }
                    }
                }
            }
        }
    }
}

