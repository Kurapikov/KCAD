import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id : kcad_main_window
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Text {
        id: text1
        x: 22
        y: 15
        color: "#ff0000"
        text: qsTr("Text")
        font.pixelSize: 12
    }

    Text {
        id: text2
        x: 601
        y: 15
        color: "#ff0000"
        text: qsTr("Text")
        font.pixelSize: 12
    }

    Text {
        id: text3
        x: 22
        y: 454
        color: "#ff0000"
        text: qsTr("Text")
        font.pixelSize: 12
    }

    Text {
        id: text4
        x: 601
        y: 454
        color: "#ff0000"
        text: qsTr("Text")
        font.pixelSize: 12
    }
}
