import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import predator_rgb

Rectangle {
    id: root

    property string title: ""
    property bool showTitle: title.length > 0

    color: Theme.lighterBackground
    radius: 0
    border.color: Theme.border
    border.width: 1

    default property alias content: contentLayout.data

    implicitHeight: contentLayout.implicitHeight + 32

    ColumnLayout {
        id: contentLayout
        anchors.fill: parent
        anchors.margins: 16
        spacing: 16

        Text {
            visible: root.showTitle
            text: root.title.toUpperCase()
            color: Theme.foreground
            font.pixelSize: 16
            font.weight: Font.Bold
            font.letterSpacing: 1.0
            Layout.bottomMargin: 4
        }
    }

    Behavior on border.color {
        ColorAnimation { duration: 200 }
    }
}
