import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import predator_rgb

ColumnLayout {
    id: root

    property int hue: 0
    property real saturation: 0.0
    property real value: 1.0
    property color currentColor: "#FFFFFF"
    property string hexText: "#FFFFFF"
    property int selectedZone: 0

    signal colorChanged(int hue, real saturation, real value)

    spacing: 16

    Text {
        text: "COLOR PICKER"
        color: Theme.foreground
        font.pixelSize: 16
        font.weight: Font.Bold
        font.letterSpacing: 1.0
    }

    RowLayout {
        spacing: 16
        Layout.fillWidth: true

        HueSlider {
            Layout.preferredWidth: 24
            Layout.preferredHeight: 200
            hue: root.hue
            onSelected: (hue) => root.colorChanged(hue, root.saturation, root.value)
        }

        Rectangle {
            width: 200
            height: 200
            color: Theme.lighterBackground
            border.color: Theme.border
            border.width: 1
            Layout.alignment: Qt.AlignTop

            SvBox {
                anchors.fill: parent
                anchors.margins: 4
                hueColor: Qt.hsva(root.hue / 360, 1, 1, 1)
                saturation: root.saturation
                value: root.value
                onSelected: (saturation, value) => root.colorChanged(root.hue, saturation, value)
            }
        }

        ColumnLayout {
            spacing: 8
            Layout.alignment: Qt.AlignTop

            Rectangle {
                width: 72
                height: 72
                color: root.currentColor
                border.color: Theme.border
                border.width: 1
                Layout.alignment: Qt.AlignHCenter

                Behavior on color {
                    ColorAnimation { duration: 150 }
                }
            }

            Text {
                text: root.hexText
                color: Theme.foreground
                font.pixelSize: 12
                font.family: "monospace"
                Layout.alignment: Qt.AlignHCenter
            }

            Text {
                text: root.selectedZone > 0 ? "ZONE " + root.selectedZone : "MAIN"
                color: Theme.darkForeground
                font.pixelSize: 11
                Layout.alignment: Qt.AlignHCenter
            }
        }
    }
}
