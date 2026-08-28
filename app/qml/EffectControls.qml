import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import predator_rgb

Panel {
    id: root

    property string selectedMode: "Wave"
    property int speed: 5
    property int brightness: 80
    property string direction: "Left to Right"

    signal modeEdited(string mode)
    signal speedEdited(int speed)
    signal brightnessEdited(int brightness)
    signal directionEdited(string direction)

    title: "Effect Settings"

    property bool showSpeed: selectedMode !== "Static"
    property bool showDirection: ["Wave", "Shifting", "Zoom"].indexOf(selectedMode) >= 0

    RowLayout {
        spacing: 16
        Layout.fillWidth: true

        Text {
            text: "MODE"
            color: Theme.foreground
            font.pixelSize: 13
            font.weight: Font.Bold
            Layout.preferredWidth: 90
        }

        ComboBox {
            Layout.fillWidth: true
            implicitHeight: 50
            Material.roundedScale: Material.NotRounded
            model: ["Static", "Breath", "Neon", "Wave", "Shifting", "Zoom"]
            currentIndex: Math.max(0, model.indexOf(root.selectedMode))
            onActivated: (index) => root.modeEdited(currentText)
        }
    }

    RowLayout {
        spacing: 16
        Layout.fillWidth: true
        visible: root.showSpeed

        Text {
            text: "SPEED"
            color: Theme.foreground
            font.pixelSize: 13
            font.weight: Font.Bold
            Layout.preferredWidth: 90
        }

        Slider {
            Layout.fillWidth: true
            implicitHeight: 50
            Material.roundedScale: Material.NotRounded
            from: 1
            to: 10
            value: root.speed
            onMoved: root.speedEdited(value)
        }

        Text {
            text: root.speed.toString()
            color: Theme.foreground
            font.pixelSize: 13
            font.family: "monospace"
            Layout.preferredWidth: 24
            horizontalAlignment: Text.AlignRight
        }
    }

    RowLayout {
        spacing: 16
        Layout.fillWidth: true

        Text {
            text: "BRIGHTNESS"
            color: Theme.foreground
            font.pixelSize: 13
            font.weight: Font.Bold
            Layout.preferredWidth: 90
        }

        Slider {
            Layout.fillWidth: true
            implicitHeight: 50
            Material.roundedScale: Material.NotRounded
            from: 0
            to: 100
            value: root.brightness
            onMoved: root.brightnessEdited(value)
        }

        Text {
            text: root.brightness + "%"
            color: Theme.foreground
            font.pixelSize: 13
            font.family: "monospace"
            Layout.preferredWidth: 40
            horizontalAlignment: Text.AlignRight
        }
    }

    RowLayout {
        spacing: 16
        Layout.fillWidth: true
        visible: root.showDirection

        Text {
            text: "DIRECTION"
            color: Theme.foreground
            font.pixelSize: 13
            font.weight: Font.Bold
            Layout.preferredWidth: 90
        }

        ComboBox {
            Layout.fillWidth: true
            implicitHeight: 50
            Material.roundedScale: Material.NotRounded
            model: ["Left to Right", "Right to Left"]
            currentIndex: Math.max(0, model.indexOf(root.direction))
            onActivated: (index) => root.directionEdited(currentText)
        }
    }
}
