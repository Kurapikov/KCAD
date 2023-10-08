import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    id : kcad_main_window
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")

    Item {
    width:  100
    height: 100
        bgfx_bridge {
            SequentialAnimation on t {
                NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
                NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
                loops: Animation.Infinite
                running: true
            }
        }
    }
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
