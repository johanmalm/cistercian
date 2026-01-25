import QtQuick
import QtQuick.Window
import QtQuick.Controls

Window {
    id: panel
    width: 400
    height: 40
    visible: true
    color: "gray"

    Row {
        anchors.fill: parent
        spacing: 5

        Repeater {
            model: taskbar

            delegate: Rectangle {
                width: 80
                height: parent.height
                color: toplevel.activated ? "blue" : "gray"
                border.color: "white"
                radius: 4

                Text {
                    anchors.centerIn: parent
                    text: toplevel.title
                    color: "white"
                    font.pixelSize: 14
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: toplevel.activate()
                }
            }
        }
    }
}

