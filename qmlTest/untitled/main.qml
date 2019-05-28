import QtQuick 2.12
import QtQuick.Window 2.12

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    BorderImage {
        id: borderImage
        x: 175
        y: 137
        width: 268
        height: 186
        source: "C:/Users/Administrator/Pictures/微信图片_20190103171142.jpg"
    }
}
