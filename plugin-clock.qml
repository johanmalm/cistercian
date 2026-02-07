import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Rectangle {
    id: clock
    width: 100
    Layout.fillHeight: true
    color: "#20000000"
    //border.width: 1
    //border.color: "#FF888888"
    radius: 4

    Text {
        id: timeDisplay
        anchors.centerIn: parent
        font.pixelSize: 16
        color: "white"
        text: Qt.formatTime(new Date(), "hh:mm")

        // Update the time every 60 seconds
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
