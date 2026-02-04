import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: clock
    width: 100
    height: 40
    color: "transparent"

    Text {
        id: timeDisplay
        anchors.centerIn: parent
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHLeft
        font.pixelSize: 16
        color: "white"
        text: Qt.formatTime(new Date(), "hh:mm")

        Timer {
            interval: 60000
            running: true
            repeat: true
            onTriggered: {
                timeDisplay.text = Qt.formatTime(new Date(), "hh:mm")
            }
        }
    }
}
